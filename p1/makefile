CC=gcc
CFLAGS=-Wall -g
OBJM=slist.o mergesort.o hashmap.o
OBJS=pagerank.o graph.o
OBJSC=searchPagerank.o 

all: pagerank searchPagerank

searchPagerank : $(OBJSC) $(OBJM)
	$(CC) -o searchPagerank $(OBJSC) $(OBJM)

pagerank : $(OBJS) $(OBJM)
	$(CC) -o pagerank $(OBJS) $(OBJM)

searchPagerank.o : searchPagerank.c mergesort.h hashmap.h slist.h

pagerank.o : pagerank.c slist.h graph.h mergesort.h hashmap.h

slist.o : slist.c slist.h

graph.o : graph.c graph.h slist.h

hashmap.o : hashmap.c hashmap.h slist.h

mergesort.o : mergesort.c mergesort.h

clean :
	rm -f pagerank searchPagerank *.o core
