CC=gcc
CFLAGS=-Wall -g
OBJSM=slist.o mergesort.o hashmap.o
OBJSB2=normalise.o
OBJSA=pagerank.o graph.o
OBJSB=inverted.o
OBJSC=searchPagerank.o 
OBJS2=searchTfIdf.o 
OBJS3=scaledFootrule.o

EXES=pagerank inverted searchPagerank searchTfIdf scaledFootrule

all: $(EXES)



scaledFootrule : $(OBJS3) $(OBJSM)
	$(CC) -o scaledFootrule $(OBJS3) $(OBJSM)

searchTfIdf : $(OBJS2) $(OBJSB2) $(OBJSM)
	$(CC) -lm -o searchTfIdf $(OBJS2) $(OBJSB2) $(OBJSM)

searchPagerank : $(OBJSC) $(OBJSM)
	$(CC) -o searchPagerank $(OBJSC) $(OBJSM)

inverted : $(OBJSB) $(OBJSB2) $(OBJSM)
	$(CC) -o inverted $(OBJSB) $(OBJSB2) $(OBJSM)

pagerank : $(OBJSA) $(OBJSM)
	$(CC) -o pagerank $(OBJSA) $(OBJSM)



searchPagerank.o : searchPagerank.c mergesort.h hashmap.h slist.h

inverted.o : inverted.c slist.h mergesort.h hashmap.h slist.h normalise.h

pagerank.o : pagerank.c slist.h graph.h mergesort.h hashmap.h

searchTfIdf.o : searchTfIdf.c mergesort.h hashmap.h slist.h normalise.h

scaledFootrule.o : scaledFootrule.c mergesort.h hashmap.h slist.h 


slist.o : slist.c slist.h

graph.o : graph.c graph.h slist.h

hashmap.o : hashmap.c hashmap.h slist.h

mergesort.o : mergesort.c mergesort.h

normalise.o : normalise.c normalise.h


clean :
	rm -f $(EXES) $(OBJSM) $(OBJSB2) $(OBJSA) $(OBJSB) $(OBJSC) $(OBJS2) $(OBJS3)
