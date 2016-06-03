#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "mergesort.h"
#include "hashmap.h"

#define BUFF_SIZE 200
#define MAX_DISPLAY 10

int main(int argc, char **argv) {
	//initialise file i/o
	FILE *iI = fopen("invertedIndex.txt", "r");
	FILE *pL = fopen("pagerankList.txt", "r"); 
	if(!(iI && pL)){
		fprintf(stderr, "Error opening file");
		return EXIT_FAILURE;
	}

	//read from pagerankList.txt
	slist l = newList((void*)strdup, free);

	char buffer[BUFF_SIZE];
	while(fgets(buffer, BUFF_SIZE - 1, pL)) {		
		char *c = strchr(buffer, ',');
		if(!c) continue;
		*c = '\0';
		listEnter(l, buffer);
	}
	
	//move values from linked list to hashmap for faster use
	unsigned int len = listLength(l);
	Hashmap urls = newHashmap((len*3)/2);
	
	for(;hasNext(l); listNext(l)) {
		mapInsert(urls, (char*)readList(l), 0);
	}
	listReset(l);

	mergesort((void**)(argv + 1), argc - 1, (int (*)(void*, void*))strcmp, 0);
	
	int i = 1;
	while(i < argc && !feof(iI)) {
		fscanf(iI, "%s", buffer);
		if (strcmp(buffer, argv[i])) {

			//read to end of line
			while(fgetc(iI) != '\n' && !feof(iI));

		} else {
			i++;

			FILE *tmp = tmpfile();
			do {
				fgets(buffer, BUFF_SIZE - 1, iI);
				fprintf(tmp, "%s", buffer);
			} while (!feof(iI) && !strchr(buffer, '\n'));

			rewind(tmp);
			//increment all of the files who have the word
			while (fscanf(tmp, "%s", buffer) != EOF) {
				
				mapIncrement(urls, buffer);
			}
			fclose(tmp);
		}
	}

	int count = 0;
	if (i == argc){
		for(listReset(l); hasNext(l) && count < MAX_DISPLAY; listNext(l)){

			//only the pages who have been inremented for all terms can be printed
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
