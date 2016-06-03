#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "slist.h"
#include "graph.h"
#include "hashmap.h"
#include "mergesort.h"

#define BUFF_SIZE 70
#define EPS 1E-6

typedef struct _url * Url;

typedef struct _url {
	char *name;
	double pRank;
	int outdeg;
	char notActuallyZero;
} url;

static int urlComp(void* a, void* b){
	if(((Url)a)->pRank < ((Url)b)->pRank - EPS) return -1;
	if(((Url)a)->pRank > ((Url)b)->pRank + EPS) return 1;
	if(((Url)a)->notActuallyZero * ((Url)a)->outdeg < ((Url)b)->notActuallyZero * ((Url)b)->outdeg) return -1;
	return ((Url)a)->notActuallyZero * ((Url)a)->outdeg > ((Url)b)->notActuallyZero * ((Url)b)->outdeg;
}

static void print(Url *input, FILE *output, int length){
	int i;
	for(i = 0; i < length; i++){
		fprintf(output, "%s, %d, %.8lf\n", input[i]->name, input[i]->notActuallyZero * input[i]->outdeg, input[i]->pRank);
	}
	return;
}

int main(int argc, char **argv){

	//check usage
	if(argc != 4){
		fprintf(stderr, "usage: \"./pagerank\" damping_factor diffPR maxIterations");
		return EXIT_FAILURE;
	}

	//initialise i/o
	FILE *fin = fopen("collection.txt","r");
	FILE *fout = fopen("pagerankList.txt","w");
	double d, diffPR;
	int maxIterations;
	slist urls = newList((void*)strdup, free);
	d = atof(argv[1]);
	diffPR = atof(argv[2]);
	maxIterations = atoi(argv[3]);

	//read collection.txt
	char buffer[BUFF_SIZE];
	while(fscanf(fin, "%s", buffer) != EOF){
		assert(buffer[0]);
		listEnter(urls, buffer);
	}
	
	int len = listLength(urls);
	Url *aurls = malloc(len * sizeof(url));
	Graph g = newGraph(len);
	Hashmap m = newHashmap((len*3)/2);
	
	//convert llist to array for faster reading
	int i;
	for(i = 0; i < len; i++){
		aurls[i] = malloc(sizeof(url));
		aurls[i]->name = (char*)readList(urls);
		aurls[i]->pRank = (1/(double)len);
		aurls[i]->notActuallyZero = 1;
		mapInsert(m, aurls[i]->name, i);
		listNext(urls);
	}
	listReset(urls);

	//build graph
	for(i = 0; i < len; i++){
		strcpy(buffer, aurls[i]->name);
		strcat(buffer, ".txt");
		FILE *webpage = fopen(buffer, "r");
		char *seen = calloc(len, sizeof(char));
		int nOutgoingLinks = 0;

		fscanf(webpage, "#start Section-1");
		while (fscanf(webpage, "%s", buffer) != EOF){
			if(!strcmp(buffer, "#end")){
				break;
			}

			int j = mapSearch(m, buffer);
			if(j != -1 && j != i && !seen[j]){
				insertEdge(g,j,i);
				seen[j] = 1;
				nOutgoingLinks++;
			}
		}

		if(!nOutgoingLinks){
			int j;
			for(j = 0; j < len; j++){
				if(j != i) {
					insertEdge(g, j, i);
				}
			}
			nOutgoingLinks = len - 1;
			aurls[i]->notActuallyZero = 0;
		}
		aurls[i]->outdeg = nOutgoingLinks;
		fclose(webpage);
		free(seen);
	}
	
	dropMap(m);

	//calculate pagerank
	double diff = diffPR, *newPRanks = malloc(len * sizeof(double));
	for(i = 0; i < maxIterations && diff > diffPR - EPS; i++){
		diff = 0;
		
		int j;
		for(j = 0; j < len; j++){
			double sum = 0;

			slist inUrls;
			for(inUrls = GetAdjacencies(g, j); hasNext(inUrls); listNext(inUrls)){
				int val = *(int*)readList(inUrls);
				sum += aurls[val]->pRank / aurls[val]->outdeg;
			}
			listReset(inUrls);

			sum *= d;
			sum += (1 - d)/len;
			diff += fabs(aurls[j]->pRank - sum);
			newPRanks[j] = sum;
		}
		for(j = 0; j < len; j++) {
			aurls[j]->pRank = newPRanks[j];
		}
	}
	free(newPRanks);			

	mergesort((void**)aurls, len, urlComp, 1);
	print(aurls, fout, len);
	
	for(i = 0; i < len; i++){
		free(aurls[i]);
	}

	free(aurls);
	fclose(fin);
	fclose(fout);
	freeList(urls);
	dropGraph(g);
	return EXIT_SUCCESS;
}


