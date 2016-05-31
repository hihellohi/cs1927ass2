#include <stdlib.h>
#include "mergesort.h"

static void sort(void **a, void **b, int parity, int start, int end, int (*comp)(void*, void*), char revr){
	if(start == end - 1) {
		return;
	}	

	sort(a,b,parity+1,start,(start + end) >> 1, comp, revr);
	sort(a,b,parity+1,(start + end) >> 1, end, comp, revr);

	if(parity % 2) {
		void **tmp = a;
		a = b;
		b = tmp;
	}

	int lower = start, upper = (start + end) >> 1, i;
	for (i = start; lower < (start + end) >> 1 && upper < end; i++) {
		int tmp = comp(a[lower], a[upper]);
		if(!tmp || ((tmp > 0) == (revr != 0))){
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

void mergesort(void **list, int length, int (*comp)(void*, void*), char revr){
	if(!length) return;
	void **a = malloc(sizeof(void*) * length);
	
	int i;
	for (i = 0; i < length; i++) {
		a[i] = list[i];
	}

	sort(a, list, 0, 0, length, comp, revr);
	free(a);
	return;
}
