#ifndef HASHMAP
#define HASHMAP
typedef struct _hashmap * Hashmap;

Hashmap newHashmap(int);
void mapInsert(Hashmap, char *key, int value);
int mapSearch(Hashmap, char *key);
void dropMap(Hashmap);

#endif
