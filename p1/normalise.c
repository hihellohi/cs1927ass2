#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "normalise.h"

char *normalise(char *string){
	int i;
	char *c;
	for (i = 0, c = string; !isalnum(*c) && *c; i++, c++);
	for (; string[i]; i++) string[i] = tolower(string[i]);
	for (i--; !isalnum(string[i]) && i >= c - string; i--) string[i] = 0;
	return string;
}
