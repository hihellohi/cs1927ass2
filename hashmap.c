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

// Creates new hashmap with an array of length nSlots
Hashmap newHashmap(int nSlots){
	Hashmap new = malloc(sizeof(hashmap));
	new->nItems = 0;
	new->nSlots = nSlots;
	new->data = calloc(nSlots, sizeof(slist));

	return new;
}

// m[key] = value
void mapInsert(Hashmap m, char *key, int value){
	char found = 0;
	unsigned int h = hash(key, m->nSlots);
	if(!m->data[h]) m->data[h] = newList(itemCopy, itemFree);
	
	slist l;
	for(l = m->data[h]; hasNext(l); listNext(l)) {
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
		m->nItems++;
	}

	listReset(l);
	return;
}

// m[key]++
void mapIncrement(Hashmap m, char *key) {
	slist l;
	unsigned int h = hash(key, m->nSlots);
	if(!m->data[h]) return;

	for(l = m->data[h]; hasNext(l); listNext(l)) {
		listItem *val = ((listItem*)readList(l));
		if(!strcmp(val->key, key)){
			val->value++;
			break;
		}
	}
	listReset(l);
}

// m[key]
int mapSearch(Hashmap m, char *key) {
	slist l;
	unsigned int h = hash(key, m->nSlots);
	if(!m->data[h]) return -1;
	
	int out = -1;
	for(l = m->data[h]; hasNext(l); listNext(l)) {
		listItem *val = ((listItem*)readList(l));
		if(!strcmp(val->key, key)){
			out = val->value;
			break;
		}
	}
	listReset(l);
	return out;
}

// frees the map
void dropMap(Hashmap m) {

	int i;
	for(i = 0; i < m->nSlots; i++){
		if(m->data[i]) freeList(m->data[i]);
	}
	free(m->data);
	free(m);
}

// makes a copy of the map
Hashmap mapCopy(Hashmap m) {

	Hashmap new = newHashmap(m->nSlots);
	if(!m->nItems) return new;
	
	int i;
	for(i = 0; i < m->nSlots; i++){
		if(!m->data[i]) continue;

		slist l;
		for(l = m->data[i]; hasNext(l); listNext(l)){
			listItem *val = ((listItem*)readList(l));
			mapInsert(new, val->key, val->value);
		}
		listReset(l);
	}
	return new;
}


int mapSize(Hashmap m){
	
	return m->nItems;
}

// returns the percentage of the map being used
double mapCapacity(Hashmap m) {

	return (double)m->nItems/m->nSlots;
}

// doubles the number of slots in the map
void mapDouble(Hashmap m) {
	
	slist *old = m->data;
	m->nSlots <<= 1;
	m->nItems = 0;
	m->data = calloc(m->nSlots, sizeof(slist));

	int i;
	for(i = 0; i < m->nSlots >> 1; i++){
		if(!old[i]) continue;
		slist l;
		for(l = old[i]; hasNext(l); listNext(l)) {
			listItem *val = ((listItem*)readList(l));
			mapInsert(m, val->key, val->value);
		}
		freeList(l);
	}
	free(old);
}

