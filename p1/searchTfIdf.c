#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "hashmap.h"
#include "slist.h"
#include "mergesort.h"

#define BUFF_SIZE 1000

typedef struct _url * Url;

typedef struct _url {
	char *name;
	double tfidf;
} url;

int main(int argc, char **argv){
	argc--;
	argv++;

	// create hashmap to turn words back to their indicies
	Hashmap m = newHashmap((argc*3)/2);

	int i;
	for(i = 0; i < argc; i++){
		mapInsert(m, argv[i], i);
	}

	// read from collection.txt
	FILE *collection = fopen("collection.txt", "r");
	if(!collection) {
		perror("Error opening collection.txt\n");
		return EXIT_FAILURE;
	}

	slist l = newList((void*)strdup, free);
	char buffer[BUFF_SIZE];
	while(fscanf(collection, "%s", buffer) != EOF){
		assert(buffer[9]);
		listEnter(l, buffer);
	}

	// calculate idf
	FILE *inverted = fopen("invertedIndex.txt", "r");
	int len = listLength(l);	
	double *idf = calloc(argc, sizeof(double));

	while(!feof(inverted)) {
		fscanf(inverted, "%s", buffer);
		int index = mapSearch(m, buffer);
		if(index == -1){
			
			//read to end of line
			while(fgetc(inverted) != '\n' && !feof(inverted));

		} else {

			int j;
			for(j = 0; fgetc(inverted) != '\n' && !feof(inverted); j++){
				fscanf(inverted, "%s", buffer);
			}

			assert(j);
			idf[index] = log((double)len/(double)j);
		}
	}

	for(i = 0; i < argc; i++){
		printf("%s: %.6lf\n", argv[i], idf[i]);
	}

	fclose(collection);
	freeList(l);
	free(idf);
	dropMap(m);
	return EXIT_SUCCESS;
}
