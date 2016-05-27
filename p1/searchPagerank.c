#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "mergesort.h"
#include "hashmap.h"

#define BUFF_SIZE 1000
#define MAX_DISPLAY 10

int main(int argc, char **argv) {
	FILE *iI = fopen("invertedIndex.txt", "r");
	FILE *pL = fopen("pagerankList.txt", "r"); 
	if(!(iI && pL)){
		fprintf(stderr, "Error opening file");
		return EXIT_FAILURE;
	}

	slist l = newList((void*)strdup, free);

	char buffer[BUFF_SIZE];
	while(fgets(buffer, BUFF_SIZE, pL)) {		
		char *c = strchr(buffer, ',');
		*c = '\0';
		listEnter(l, buffer);
	}
	
	unsigned int len = listLength(l);
	Hashmap urls = newHashmap((len*3)/2);
	
	for(;hasNext(l); listNext(l)) {
		char *val = (char*)readList(l);
		mapInsert(urls, val, 0);
	}
	listReset(l);

	mergesort((void**)(argv + 1), argc - 1, (int (*)(void*, void*))strcmp, 1);
	
	int i = 1;
	while(i < argc && !feof(iI)) {
		fscanf(iI, "%s", buffer);
		if (strcmp(buffer, argv[i])) {
			while(fgetc(iI) != '\n' && !feof(iI));
		} else {
			i++;
			while(fgetc(iI) != '\n' && !feof(iI)) {
				fscanf(iI, "%s", buffer);
				mapIncrement(urls, buffer);
			}
		}
	}

	int count = 0;
	if (i == argc){
		for(listReset(l); hasNext(l) && count < MAX_DISPLAY; listNext(l)){
			if (mapSearch(urls, (char*)readList(l)) == argc - 1){
				printf("%s\n", (char*)readList(l));
				count++;
			}
		}
	}

	dropMap(urls);
	freeList(l);
	fclose(pL);
	fclose(iI);
	return EXIT_SUCCESS;
}
