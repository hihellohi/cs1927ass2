#ifndef HASHMAP
#define HASHMAP
typedef struct _hashmap * Hashmap;

Hashmap newHashmap(int);
void mapIncrement(Hashmap, char*);
void mapInsert(Hashmap, char *key, int value);
int mapSearch(Hashmap, char *key);
void dropMap(Hashmap);
double mapCapacity(Hashmap);
void mapDouble(Hashmap);
int mapSize(Hashmap);
Hashmap mapCopy(Hashmap);

#endif
