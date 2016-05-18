#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "slist.h"

typedef struct _node * node;

struct _node {
	void *value
	node next;
};

struct _slist {
	node first;
	node *last;
	node cur;
	(void*)copy(void*);
	(void)free(void*);
	int num;
};



slist newList((void*)copy(void*), (void)fin(void*)) {
	slist out = malloc(sizeof(struct _slist));
	out->first = NULL;
	out->last = &(out->first);
	out->num = 0;
	out->comp = comp;
	out->free = fin;
	return out;
}

void listEnter(slist list, const void *input){
	node new = malloc(sizeof(struct _node));
	if(!list->first) list->cur = new;
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

static void mergesort((signed char)comp(void*, void*), node *a, node *b, int parity, int start, int end){
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
		if (comp(a[lower], a[upper]) > 0) {
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
	mergesort(list->comp,a,b,0,0,list->num);
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
