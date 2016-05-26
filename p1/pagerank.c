#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "slist.h"
#include "graph.h"

#define BUFF_SIZE 100
#define EPS 1E-6

typedef struct _url {
	char *name;
	double pRank;
	int outdeg;
} url;

static void *stringCopy(void *x){
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
		if (a[lower].pRank > a[upper].pRank + EPS) {
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
		fprintf(output, "%s, %d, %.8lf\n", input[i].name, input[i].outdeg, input[i].pRank);
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
	FILE *fout = fopen("pagerank_list.txt","w");
	double d, diffPR;
	int maxIterations;
	slist urls = newList(stringCopy, free);
	d = atof(argv[1]);
	diffPR = atof(argv[2]);
	maxIterations = atoi(argv[3]);

	//read collection.txt
	char buffer[BUFF_SIZE];
	while(fscanf(fin, "%s", buffer) != EOF){
		if(buffer[0] == ' ' || buffer[0] == '\t' || buffer[0] == '\n'){
			continue;
		}
		listEnter(urls, buffer);
	}
	
	int len = listLength(urls);
	url *aurls = malloc(len * sizeof(url));
	Graph g = newGraph(len);
	
	//convert llist to array for faster reading
	int i;
	for(i = 0; i < len; i++){
		aurls[i].name = (char*)readList(urls);
		aurls[i].pRank = (1/(double)len);
		listNext(urls);
	}

	//build graph
	for(i = 0; i < len; i++){
		char filename[BUFF_SIZE];
		strcpy(filename, aurls[i].name);
		strcat(filename, ".txt");
		FILE *webpage = fopen(filename, "r");
		char *seen = calloc(len, sizeof(char));
		int nOutgoingLinks = 0;

		fscanf(webpage, "#start Section-1");
		while (fscanf(webpage, "%s", buffer) != EOF){
			if(buffer[0] == ' ' || buffer[0] == '\t' || buffer[0] == '\n'){
				continue;
			}
			if(!strcmp(buffer, "#end")){
				break;
			}

			//USE HASHMAP TIME ALLOWING
			int j;
			for(j = 0; j < len; j++){
				if(!strcmp(buffer, aurls[j].name)){
					if(j != i && !seen[j]){
						insertEdge(g,j,i);
						seen[j] = 1;
						nOutgoingLinks++;
					}
					break;
				}
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
		}
		aurls[i].outdeg = nOutgoingLinks;
		fclose(webpage);
		free(seen);
	}

	double diff = diffPR, *newPRanks = malloc(len * sizeof(double));
	for(i = 0; i < maxIterations && diff > diffPR - EPS; i++){
		diff = 0;
		
		int j;
		for(j = 0; j < len; j++){
			double sum = 0;

			slist inUrls;
			for(inUrls = GetAdjacencies(g, j); hasNext(inUrls); listNext(inUrls)){
				int val = *(int*)readList(inUrls);
				sum += aurls[val].pRank / aurls[val].outdeg;
			}
			listReset(inUrls);

			sum *= d;
			sum += (1 - d)/len;
			diff += abs(aurls[j].pRank - sum);
			newPRanks[j] = sum;
		}
		for(j = 0; j < len; j++) {
			aurls[j].pRank = newPRanks[j];
		}
	}
	free(newPRanks);			

//	//print list (temporary)
//	for(i = 0; i < len; i++){
//		printf("%s, deg: %d, PR: %lf <- ", aurls[i].name, aurls[i].outdeg, aurls[i].pRank);
//
//		slist a;
//		for(a = GetAdjacencies(g, i); hasNext(a); listNext(a)){
//			printf("%s ", aurls[*(int*)readList(a)].name);
//		}
//		printf("\n");
//		listReset(a);
//	}

	sort(aurls, len);
	print(aurls, fout, len);
	
	free(aurls);
	fclose(fin);
	fclose(fout);
	freeList(urls);
	dropGraph(g);
	return EXIT_SUCCESS;
}


