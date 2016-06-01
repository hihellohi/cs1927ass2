#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "mergesort.h" 
#include "hashmap.h" 
#include "slist.h" 

#define BUFF_SIZE 70
#define INITIAL_HM_SIZE 1
#define EPS 1E-6

typedef struct _url {
	char *name;
	int position;
} * Url;

static int urlComp(void *a, void *b) {
	if(((Url)a)->position < ((Url)b)->position) return -1;
	return ((Url)a)->position > ((Url)b)->position;
}

static char findAugCycle(int *out, double **cost, const Url *urls, int len, double **dCost, int **back) {
	// Algorithm to find negative cycles (swapping positions around the cycle will give you a cheaper result)

	int i, j, k;

	for(i = 0; i < len; i++) 
	for(j = 0; j < len; j++) {
			
		dCost[i][j] = cost[i][urls[j]->position] - cost[i][urls[i]->position];
		back[i][j] = j;
	}

	//Floyd-Warshall algorithm to search for negative cycles
	for(k = 0; k < len; k++)
	for(i = 0; i < len; i++)
	for(j = 0; j < len; j++)
	if(dCost[i][k] + dCost[k][j] < dCost[i][j] - EPS) {
		dCost[i][j] = dCost[i][k] + dCost[k][j];
		back[i][j] = back[i][k];
		if(i == j) {
			k = 0;
			do {
				out[k++] = i;
				i = back[i][j];
			} while(i != j);
			out[k] = -1;
			
			return 1;
		}
	}
	return 0;
}

int main(int argc, char** argv) {

	Hashmap masterM = newHashmap(INITIAL_HM_SIZE), cur;
	slist masterL = newList((void*)strdup, free);
	slist fileRanks = newList((void*)mapCopy, (void (*)(void*))dropMap);

	// get input
	Hashmap dummy = newHashmap(INITIAL_HM_SIZE);
	char buffer[BUFF_SIZE];
	int i, j;
	for (i = 1; i < argc; i++) {

		assert(!hasNext(fileRanks));
		listEnter(fileRanks, dummy);

		FILE *f = fopen(argv[i], "r");
		if(!f) {
			fprintf(stderr, "error opening %s", argv[i]);
			return EXIT_FAILURE;
		}

		while(!feof(f)){

			if(fscanf(f, "%s", buffer) == EOF) break; // file is empty

			cur = (Hashmap)readList(fileRanks);
			mapInsert(cur, buffer, mapSize(cur) + 1);
			if(mapCapacity(cur) > 0.7) mapDouble(cur);

			if(mapSearch(masterM, buffer)) {

				mapInsert(masterM, buffer, 0);
				if(mapCapacity(masterM) > 0.7) mapDouble(cur);
				listEnter(masterL, buffer);
			}			
		}

		listNext(fileRanks);
		fclose(f);
	}
	dropMap(dummy);

	//convert masterL to an array
	int len = listLength(masterL);
	Url *urls = malloc(len * sizeof(struct _url*));
	for(i = 0, listReset(masterL); hasNext(masterL); i++, listNext(masterL)) {

		urls[i] = malloc(sizeof(struct _url));
		urls[i]->name = (char*)readList(masterL);
		urls[i]->position = i;
	}

	// compute costs
	double **cost = malloc(len * sizeof(double*));
	for(i = 0; i < len; i++) {

		cost[i] = calloc(len, sizeof(double));
		for(listReset(fileRanks); hasNext(fileRanks); listNext(fileRanks)) 
			for(cur = (Hashmap)readList(fileRanks), j = 0; j < len; j++) {

				int position = mapSearch(cur, urls[i]->name); 
				if(position != -1 && mapSize(cur))
					cost[i][j] += fabs(((double)position/mapSize(cur)) - ((double)(j + 1)/len));
			}
	}

	/* BIPARTITE MATCHING
	 *
	 * Start off with an arbitrary complete matching (already done in line 106). continuously improve it by swapping until it can
	 * no longer be improved. The resulting matching is the lowest cost one.
	 *
	 * we improve the matching by creating a complete weighted directed "improvement" graph where every vertex is a url and every
	 * edge represents and is weighted by the change in the s-f-distance if we were to give the position of the destination url 
	 * to the source url
	 *
	 * If we find a negative cycle, we can reduce the cost of the matching by swapping the positions of the urls in the direction
	 * of said cycle. If no cycle can be found the current matching can no longer be improved (because of reasons) and therfore
	 * is the lowest cost one.
	 *
	 * Best Case O(n^3), Worst case O(n^4) both of which are a lot better than o(n!)
	 *
	 * ref: Nite Nimajneb "The Hitchhiker’s Guide to the Programming Contests" http://orac.amt.edu.au/notes/icpc.pdf
	 */
	
	//declare these arrays here so we don't have to make them every iteration
	double **d = malloc(len * sizeof(double*));
	int **b = malloc(len * sizeof(int*));
	for(i = 0; i < len; i++) {
		d[i] = malloc(len * sizeof(double));
		// DeltaCost[i][j] = the change in the scaled footrule distance if j's position was given to i
		b[i] = malloc(len * sizeof(int));
		// back array to find the members and the direction of the cycle
	}

	int *negCycle = malloc((len + 1) * sizeof(int));
	while (findAugCycle(negCycle, cost, urls, len, d, b)){

		// swap the urls in the direction of the cycle
		j = urls[negCycle[0]]->position;
		for(i = 1; negCycle[i] != -1; i++){
			urls[negCycle[i-1]]->position = urls[negCycle[i]]->position;
		}
		urls[negCycle[i-1]]->position = j;
	}

	// calculate the s-f-distance
	double distance = 0;
	for(i = 0; i < len; i++) {
		distance += cost[i][urls[i]->position];
	}
	
	// print out result
	mergesort((void**)urls, len, urlComp, 0); 
	printf("%.6lf\n", distance);

	// freedom!
	for(i = 0; i < len; i++) {
		printf("%s\n", urls[i]->name);
		free(cost[i]);
		free(urls[i]);
		free(d[i]);
		free(b[i]);
	}
	free(negCycle);
	free(cost);
	free(urls);
	free(d);
	free(b);
	freeList(fileRanks);
	freeList(masterL);
	dropMap(masterM);

	return EXIT_SUCCESS;
}
