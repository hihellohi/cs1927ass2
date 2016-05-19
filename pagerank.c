#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "graph.h"

#define BUFF_SIZE 100

typedef struct _url {
	char *name;
	double *pRank;
} url;

void *stringCopy(void *x){
	return strdup((char*)x);
}

static void mergesort(url *a, url *b, int parity, int start, int end){
	if(start == end - 1) {
		return;
	}	
	mergesort(a,b,parity+1,start,(start + end) >> 1);
	mergesort(a,b,parity+1,(start + end) >> 1, end);
	if(parity % 2) {
		url *tmp = a;
		a = b;
		b = tmp;
	}
	int lower = start, upper = (start + end) >> 1, i;
	for (i = start; lower < (start + end) >> 1 && upper < end; i++) {
		//if (a[lower].pRank > a[upper].pRank) {
		if(strcmp(a[lower].name, a[upper].name) > 1){
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

void sort(url *list, int length){
	if(!length) return;
	url *a = malloc(sizeof(url) * length);
	int i;
	for (i = 0; i < length; i++) {
		a[i] = list[i];
	}
	mergesort(a, list, 0, 0, length);
	free(a);
	return;
}

void print(url *input, FILE *output, int length){
	int i;
	for(i = 0; i < length; i++){
		fprintf(output, "%s\n", input[i].name);
	}
	return;
}

int main(int argc, char **argv){
	FILE *fin = fopen("collection.txt","r");
	FILE *fout = fopen("pagerank_list.txt","w");
	double d, diffPR;
	int maxIterations;
	slist urls = newList(stringCopy, free);

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

	url *aurls = malloc(listLength(urls) * sizeof(url));
	
	int i;
	for(i = 0; i < listLength(urls); i++){
		aurls[i].name = (char*)readList(urls);
		aurls[i].pRank = 0;
		listNext(urls);
	}

	Graph g = newGraph(listLength(urls));

	sort(aurls, listLength(urls));
	print(aurls, stdout, listLength(urls));
	
	free(aurls);
	fclose(fin);
	fclose(fout);
	freeList(urls);
	return EXIT_SUCCESS;
}


