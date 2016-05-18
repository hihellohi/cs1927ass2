
#ifndef SLIST
#define SLIST

typedef struct _slist * slist;

slist newList();
void listEnter(slist list, const char *input);
void printList(slist list);
void freeList(slist list);
void sortlist(slist list);

#endif
