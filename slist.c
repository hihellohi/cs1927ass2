#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "slist.h"

typedef struct _node * node;

struct _node {
	void *value;
	node next;
};

struct _slist {
	node first;
	node *last;
	node cur;
	void *(*copy)(void*);
	void (*free)(void*);
	int num;
};

void *readList(slist list) {
	return list->cur->value;
}

void listReset(slist list) {
	list->cur = list->first;
}

char hasNext(slist list) {
	return list->cur != NULL;
}

char listNext(slist list) {
	if (!list->cur) return 0;
	list->cur = list->cur->next;
	return 1;
}
	
int listLength(slist list){
	return list->num;
}

slist newList(void *(*copy)(void*), void (*fin)(void*)) {
	slist out = malloc(sizeof(struct _slist));
	out->first = out->cur = NULL;
	out->last = &(out->first);
	out->num = 0;
	out->free = fin;
	out->copy = copy;
	return out;
}

void listEnter(slist list, void *input){
	node new = malloc(sizeof(struct _node));
	if(!list->cur) list->cur = new;
	*(list->last) = new;
	list->last = &(new->next);
	new->value = list->copy(input);
	new->next = NULL;
	list->num++;
	return;
}

void freeList(slist list){
	node cur,tmp;
	for(cur = list->first; cur; cur = tmp) {
		list->free(cur->value);
		tmp = cur->next;
		free(cur);
	}
	free(list);
	return;
}

