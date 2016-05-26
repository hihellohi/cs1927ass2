#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "mergesort.h"
#include "hashmap.h"

#define BUFF_SIZE 1000

int main(int argc, char **argv) {
	FILE *iI = fopen("invertedIndex.txt", "r");
	FILE *pL = fopen("pagerankList.txt", "r"); 
	if(!(iI && pL)) return EXIT_FAILURE;

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

	mergesort((void**)(argv + 1), argc - 1, (int (*)(void*, void*))strcmp, 1);
	
	int i;
	for(i = 1; i < argc; i++){
		printf("%s\n", argv[i]);
	}

	dropMap(urls);
	freeList(l);
	fclose(pL);
	fclose(iI);
	return EXIT_SUCCESS;
}
