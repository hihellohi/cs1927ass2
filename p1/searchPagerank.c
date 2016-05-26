#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"

int main(int argc, char **argv) {
	FILE *iI = fopen("invertedIndex.txt", "r");
	FILE *pL = fopen("pagerankList.txt", "r"); 
	fclose(pL);
	fclose(iI);
	return EXIT_SUCCESS;
}
