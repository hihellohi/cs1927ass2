CC=gcc
CFLAGS=-Wall -g
OBJSAC2=slist.o mergesort.o hashmap.o
OBJSA=pagerank.o graph.o
OBJSC=searchPagerank.o 
OBJS2=searchTfIdf.o

all: pagerank searchPagerank searchTfIdf



searchTfIdf : $(OBJS2) $(OBJSAC2)
	$(CC) -lm -o searchTfIdf $(OBJS2) $(OBJSAC2)

searchPagerank : $(OBJSC) $(OBJSAC2)
	$(CC) -o searchPagerank $(OBJSC) $(OBJSAC2)

pagerank : $(OBJSA) $(OBJSAC2)
	$(CC) -o pagerank $(OBJSA) $(OBJSAC2)



searchPagerank.o : searchPagerank.c mergesort.h hashmap.h slist.h

pagerank.o : pagerank.c slist.h graph.h mergesort.h hashmap.h

searchTfIdf.o : searchTfIdf.c mergesort.h hashmap.h slist.h



slist.o : slist.c slist.h

graph.o : graph.c graph.h slist.h

hashmap.o : hashmap.c hashmap.h slist.h

mergesort.o : mergesort.c mergesort.h



clean :
	rm -f pagerank searchPagerank *.o core
