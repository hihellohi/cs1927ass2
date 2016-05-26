#ifndef SLIST
#define SLIST

typedef struct _slist * slist;

slist newList(void *(*copy)(void*), void (*fin)(void*)); 
char hasNext(slist list);
void *readList(slist list);
void listReset(slist list);
char listNext(slist list);
int listLength(slist list);
void listEnter(slist list, void *input);
void freeList(slist list);

#endif
