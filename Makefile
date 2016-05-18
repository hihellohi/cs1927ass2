CC=gcc
CFLAGS=-Wall -g
OBJS=slist.o pagerank.o graph.o

pagerank : $(OBJS)
	$(CC) -o pagerank $(OBJS)

pagerank.o : pagerank.c slist.h graph.h

slist.o : slist.c slist.h

graph.o : graph.c graph.h

clean :
	rm -f pagerank $(OBJS) core
