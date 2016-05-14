#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 100

typedef struct _node * node;

struct _node {
	char *name;
	double prank;	
	node next;
};

typedef struct _slist {
	node first;
	node *last;
	int num;
} *slist;

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

void mergesort(node *a, node *b, int parity, int start, int end){
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

int main(int argc, char **argv){
	FILE *fin = fopen("collection.txt","r");
	FILE *fout = fopen("pagerank_list.txt","w");
	double d, diffPR;
	int maxIterations;
	slist urls = malloc(sizeof(struct _slist));
	urls->first = NULL;
	urls->last = &(urls->first);
	urls->num = 0;

	if(argc != 4){
		fprintf(stderr, "usage: \"./pagerank\" damping_factor diffPR maxIterations");
		return EXIT_FAILURE;
	}
	d = atof(argv[1]);
	diffPR = atof(argv[2]);
	maxIterations = atoi(argv[3]);

	char buffer[BUFF_SIZE];
	while(fscanf(fin, "%s", buffer) != EOF){
		if(buffer[0] == ' ' || buffer[0] == '\t' || buffer[0] == '\n'){
			continue;
		}
		listEnter(urls, buffer);
	}

	sortlist(urls);
	printList(urls);

	fclose(fin);
	fclose(fout);
	freeList(urls);
	return EXIT_SUCCESS;
}


