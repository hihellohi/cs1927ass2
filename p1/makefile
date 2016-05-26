CC=gcc
CFLAGS=-Wall -g
OBJS=slist.o pagerank.o graph.o hashmap.o
OBJSC=searchPagerank.o slist.o

all: pagerank searchPagerank

searchPagerank : $(OBJSC)
	$(CC) -o searchPagerank $(OBJSC)

pagerank : $(OBJS)
	$(CC) -o pagerank $(OBJS)

searchPagerank.o : searchPagerank.c

pagerank.o : pagerank.c slist.h graph.h

slist.o : slist.c slist.h

graph.o : graph.c graph.h slist.h

hashmap.o : hashmap.c hashmap.h slist.h

clean :
	rm -f pagerank searchPagerank *.o core
