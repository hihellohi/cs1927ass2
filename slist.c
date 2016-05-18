#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "slist.h"

typedef struct _node * node;

struct _node {
	char *name;
	double prank;	
	node next;
};

struct _slist {
	node first;
	node *last;
	node cur;
	int num;
};


char listNext(slist list) {
	if (!list->cur) return 0;
	list->cur = list->cur->next;
	return list->cur != NULL;
}

void listReset(slist list) {
	list->cur = list->first;
}

char* listGetName(slist list) {
	return list->cur ? list->cur->name : NULL;
}

void listSetValue(slist list, double val) {
	if(list->cur) return;
	list->cur->prank = val;
}

slist newList() {
	slist out = malloc(sizeof(struct _slist));
	out->first = NULL;
	out->last = &(out->first);
	out->num = 0;
	return out;
}

void listEnter(slist list, const char *input){
	node new = malloc(sizeof(struct _node));
	*(list->last) = new;
	list->last = &(new->next);
	new->name = strdup(input);
	new->prank = 0;
	new->next = NULL;
	list->num++;
	return;
}

void printList(slist list){
	node cur;
	for(cur = list->first; cur; cur = cur->next) {
		printf("%s\n", cur->name);
	}
	return;
}

void freeList(slist list){
	node cur,tmp;
	for(cur = list->first; cur; cur = tmp) {
		free(cur->name);
		tmp = cur->next;
		free(cur);
	}
	free(list);
	return;
}

static void mergesort(node *a, node *b, int parity, int start, int end){
	if(start == end - 1) {
		return;
	}	
	mergesort(a,b,parity+1,start,(start + end) >> 1);
	mergesort(a,b,parity+1,(start + end) >> 1, end);
	if(parity % 2) {
		node *tmp = a;
		a = b;
		b = tmp;
	}
	int lower = start, upper = (start + end) >> 1, i;
	for (i = start; lower < (start + end) >> 1 && upper < end; i++) {
		if (a[lower]->prank > a[upper]->prank) {
			b[i] = a[lower];
			lower++;
		} else {
			b[i] = a[upper];
			upper++;
		}
	}
	for (; lower < (start + end) >> 1; lower++, i++){
		b[i] = a[lower];
	}
	for (; upper < end; upper++, i++) {
		b[i] = a[upper];
	}
	return;
}

void sortlist(slist list){
	if(!list->num) return;
	node *a = malloc(sizeof(struct _node) * list->num), cur;
	node *b = malloc(sizeof(struct _node) * list->num);
	int i;
	for(i = 0, cur = list->first; cur; i++, cur = cur->next) {
		a[i] = cur;
		b[i] = cur;
	}
	mergesort(a,b,0,0,list->num);
	free(a);
	for(a = &(list->first),i = 0; i < list->num; i++, a = &((*a)->next)){
		*a = b[i];
	}
	*a = NULL;
	list->last = a;
	free(a);
	free(b);
	return;
}
