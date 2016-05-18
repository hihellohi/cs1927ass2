
#ifndef SLIST
#define SLIST

typedef struct _slist * slist;

slist newList((void*)copy(void*), (void)fin(void*)i); 
void listEnter(slist list, const char *input);
void printList(slist list, FILE*);
void freeList(slist list);
void sortlist(slist list);

#endif
