#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"

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

	freeList(l);
	fclose(pL);
	fclose(iI);
	return EXIT_SUCCESS;
}
