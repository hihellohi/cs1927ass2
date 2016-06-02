#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "hashmap.h"
#include "slist.h"
#include "mergesort.h"
#include "normalise.h"

#define BUFF_SIZE 70
#define EPS 1E-6
#define MAX_DISPLAY 10

typedef struct _url * Url;

typedef struct _url {
	char *name;
	double tfidf;
} url;

static int urlComp(void* a, void* b){
	if(((Url)a)->tfidf < ((Url)b)->tfidf - EPS) return -1;
	return abs(((Url)a)->tfidf - ((Url)b)->tfidf) < EPS;
}

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
		assert(buffer[0]);
		listEnter(l, buffer);
	}

	// calculate idf
	FILE *inverted = fopen("invertedIndex.txt", "r");
	if(!inverted) {
		perror("Error opening invertedIndex.txt\n");
		return EXIT_FAILURE;
	}
	int len = listLength(l);	
	double *idf = calloc(argc, sizeof(double));

	while(!feof(inverted)) {
		fscanf(inverted, "%s", buffer);
		int index = mapSearch(m, buffer);
		if(index == -1){
			
			// read to end of line
			while(fgetc(inverted) != '\n' && !feof(inverted));

		} else {

			FILE *tmp = tmpfile();
			do {
				fgets(buffer, BUFF_SIZE, inverted);
				fprintf(tmp, "%s", buffer);
			} while (!feof(inverted) && !strchr(buffer, '\n'));

			rewind(tmp);
				
			int j;
			for(j = 0; fscanf(tmp, "%s", buffer) != EOF; j++);
			fclose(tmp);

			assert(j);
			idf[index] = log10(len/(double)j);
		}
	}

	// calculate tfidfs and push to array for sorting
	Url *urls = malloc(len * sizeof(Url));
	for(listReset(l), i = 0; hasNext(l); listNext(l), i++) {
		urls[i] = malloc(sizeof(url));
		urls[i]->name = (char*)readList(l);
		urls[i]->tfidf = 0;

		strcpy(buffer, urls[i]->name);
		strcat(buffer, ".txt");
		FILE *f = fopen(buffer, "r");
		
		if(!f){
			fprintf(stderr, "error opening %s\n", buffer);
			return EXIT_FAILURE;
		}

		do {
			fgets(buffer, BUFF_SIZE, f);
		} while(!feof(f) && !strstr(buffer, "#end Section-1"));

		fscanf(f, "#start Section-2");
		
		while(fscanf(f, "%s", buffer) != EOF) {

			if(!strcmp(buffer, "#end")){
				break;
			}

			char *c = normalise(buffer);

			int index = mapSearch(m, c);
			if(index != -1){
				urls[i]->tfidf += idf[index];
			}
		}

		fclose(f);
	}
	
	mergesort((void**)urls, len, urlComp, 1);

	for(i = 0; i < ((len < MAX_DISPLAY) ? len : MAX_DISPLAY); i++){
		printf("%s %.6lf\n", urls[i]->name, urls[i]->tfidf);
	}
	
	for(i = 0; i < len; i++){
		free(urls[i]);
	}
	
	free(urls);
	fclose(inverted);
	fclose(collection);
	freeList(l);
	free(idf);
	dropMap(m);
	return EXIT_SUCCESS;
}
