CC=gcc
CFLAGS=-Wall -g
OBJSAC23=slist.o mergesort.o hashmap.o
OBJSA=pagerank.o graph.o
OBJSC=searchPagerank.o 
OBJS2=searchTfIdf.o normalise.o
OBJS3=scaledFootrule.o

all: pagerank searchPagerank searchTfIdf scaledFootrule



scaledFootrule : $(OBJS3) $(OBJSAC23)
	$(CC) -o scaledFootrule $(OBJS3) $(OBJSAC23)

searchTfIdf : $(OBJS2) $(OBJSAC23)
	$(CC) -lm -o searchTfIdf $(OBJS2) $(OBJSAC23)

searchPagerank : $(OBJSC) $(OBJSAC23)
	$(CC) -o searchPagerank $(OBJSC) $(OBJSAC23)

pagerank : $(OBJSA) $(OBJSAC23)
	$(CC) -o pagerank $(OBJSA) $(OBJSAC23)



searchPagerank.o : searchPagerank.c mergesort.h hashmap.h slist.h

pagerank.o : pagerank.c slist.h graph.h mergesort.h hashmap.h

searchTfIdf.o : searchTfIdf.c mergesort.h hashmap.h slist.h normalise.h

scaledFootrule.o : scaledFootrule.c mergesort.h hashmap.h slist.h 


slist.o : slist.c slist.h

graph.o : graph.c graph.h slist.h

hashmap.o : hashmap.c hashmap.h slist.h

mergesort.o : mergesort.c mergesort.h

normalise.o : normalise.c normalise.h


clean :
	rm -f pagerank searchPagerank *.o core
