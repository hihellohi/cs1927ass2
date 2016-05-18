#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "graph.h"

#define BUFF_SIZE 100

void *stringCopy(void *x){
	return strdup((char*)x);
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

	int nVertices;
	
	char buffer[BUFF_SIZE];
	while(fscanf(fin, "%s", buffer) != EOF){
		if(buffer[0] == ' ' || buffer[0] == '\t' || buffer[0] == '\n'){
			continue;
		}
		listEnter(urls, buffer);
		nVertices++;
	}

	sortlist(urls);
	printList(urls, stdout);

	Graph g = newGraph(nVertices);


	fclose(fin);
	fclose(fout);
	freeList(urls);
	return EXIT_SUCCESS;
}


