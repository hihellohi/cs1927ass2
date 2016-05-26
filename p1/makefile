CC=gcc
CFLAGS=-Wall -g
OBJM=slist.o mergesort.o
OBJS=pagerank.o graph.o hashmap.o
OBJSC=searchPagerank.o 

all: pagerank searchPagerank

searchPagerank : $(OBJSC) $(OBJM)
	$(CC) -o searchPagerank $(OBJSC) $(OBJM)

pagerank : $(OBJS) $(OBJM)
	$(CC) -o pagerank $(OBJS) $(OBJM)

searchPagerank.o : searchPagerank.c

pagerank.o : pagerank.c slist.h graph.h

slist.o : slist.c slist.h

graph.o : graph.c graph.h slist.h

hashmap.o : hashmap.c hashmap.h slist.h

mergesort.o : mergesort.c mergesort.h

clean :
	rm -f pagerank searchPagerank *.o core
