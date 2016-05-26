#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "hashmap.h"

typedef struct _hashmap {
	int nItems;
	int nSlots;
	slist *data;
} hashmap;

typedef struct _listItem {
	char *key;
	int value;
} listItem;

static void itemFree(void *item){
	free(((listItem*)item)->key);
	free(item);
	return;
}

static void *itemCopy(void *item){
	listItem *new = malloc(sizeof(listItem));
	new->key = strdup(((listItem*)item)->key);
	new->value = ((listItem*)item)->value;
	return new;
}

static unsigned int hash(char *key, int n){
	unsigned int a = 84061;
	int i;
	for(i = 0; key[i]; i++){
		a *= (key[i] - ' ');
		a += i * 44809;
		a %= 104729;
	}
	return a % n;
}

Hashmap newHashmap(int nSlots){
	Hashmap new = malloc(sizeof(hashmap));
	new->nItems = 0;
	new->nSlots = nSlots;
	new->data = malloc(nSlots * sizeof(slist));

	int i;
	for(i = 0; i < nSlots; i++){
		new->data[i] = newList(itemCopy, itemFree);
	}
	return new;
}

void mapInsert(Hashmap m, char *key, int value){
	slist l;
	char found = 0;
	for(l = m->data[hash(key, m->nSlots)]; hasNext(l); listNext(l)) {
		listItem *val = ((listItem*)readList(l));
		if(!strcmp(val->key, key)){
			found = 1;
			val->value = value;
			break;
		}
	}

	if(!found){
		listItem new = {key, value};
		listEnter(m->data[hash(key, m->nSlots)], &new);
	}

	listReset(l);
	return;
}

int mapSearch(Hashmap m, char *key) {
	slist l;
	int out = -1;
	for(l = m->data[hash(key, m->nSlots)]; hasNext(l); listNext(l)) {
		listItem *val = ((listItem*)readList(l));
		if(!strcmp(val->key, key)){
			out = val->value;
			break;
		}
	}
	listReset(l);
	return out;
}

void dropMap(Hashmap m) {

	int i;
	for(i = 0; i < m->nSlots; i++){
		freeList(m->data[i]);
	}
	free(m->data);
	free(m);
}
