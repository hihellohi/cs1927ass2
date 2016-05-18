
#ifndef SLIST
#define SLIST

typedef struct _slist * slist;

slist newList();
char listNext(slist list);
void listReset(slist);
char* listGetName(slist);
void listSetValue(slist, double);
void listEnter(slist list, const char *input);
void printList(slist list, FILE*);
void freeList(slist list);
void sortlist(slist list);

#endif
