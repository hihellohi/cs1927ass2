#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "slist.h"
#include "hashmap.h"
#include "normalise.h"
#include "mergesort.h"

#define MAXURL 300

typedef struct dicti {
	char *name;
	slist address;
}dicti;
typedef struct dicti *Dictionary;

static int urlComp(void* a, void* b){
	return (strcmp(((Dictionary)a)->name,((Dictionary)b)->name));	
}

int main (void){
	FILE *fp;
	FILE *fp2;
	FILE *open;
	char url[MAXURL];
	char store[MAXURL];
	char word[MAXURL];
	char *line=malloc(sizeof(char)*1000);
	int started=0;
	int numWord=0;
	int i=0;
	int j=0;
	int temp;
	int Notseen=1;
	slist urls=newList((void*)strdup,free);
	slist vocab=newList((void*)strdup,free);
	Hashmap m=newHashmap((MAXURL*3)/2);

	if( (open=fopen("invertedIndex.txt","w"))==NULL){
		fprintf(stderr,"Error can't open invertedIndex.txt\n");
		exit(1);
	}

	if((fp=fopen("collection.txt","r"))==NULL){
		fprintf(stderr,"Can't open collection.txt");
		exit(1);
	}

	while(fscanf(fp, "%s", url) != EOF){
		listEnter(urls,url);
	}

	while(hasNext(urls)){
		strcpy(store,(char*)readList(urls));
		strcat(store,".txt");

		if((fp2=fopen(store,"r"))==NULL){
			fprintf(stderr,"Error cannot open");
			exit(1);
		}

		while (fscanf(fp2, "%s", line) != EOF){
			if((*line)==' '|| (*line)=='\n'||(*line)=='\t'){
				continue;
			}
			if(strcmp(line,"Section-2")&&started==0){
				continue;
			}		
			else if(!strcmp(line,"#end")){ // file says Setion 2
				started=0;
				break;
			}
			else if(!strcmp(line,"Section-2")){
				started=1; 
				continue;
			}
			strcpy(word,line);
			normalise(word);
			if(mapSearch(m,word)==-1){
				listEnter(vocab,word);
				mapInsert(m,word,j);
				j++;
			}
		}		
		fclose(fp2);
		listNext(urls);
	}
	listReset(urls);
	listReset(vocab);

	numWord=listLength(vocab);
	Dictionary *addressbook=malloc(sizeof(dicti)*numWord);

	for (i=0;hasNext(vocab);listNext(vocab),i++){
		addressbook[i]=malloc(sizeof(dicti));
		addressbook[i]->name=(char*)readList(vocab);
		addressbook[i]->address=newList((void*)strdup,free);
	}
	listReset(vocab);

	while(hasNext(urls)){
		strcpy(store,(char*)readList(urls));
		strcat(store,".txt");

		if((fp2=fopen(store,"r"))==NULL){
			fprintf(stderr,"Error cannot open");
			exit(1);
		}

		while (fscanf(fp2, "%s", line) != EOF){
			if((*line)==' '|| (*line)=='\n'||(*line)=='\t'){
				continue;
			}
			if(strcmp(line,"Section-2")&&started==0){
				continue;
			}		
			else if(!strcmp(line,"#end")){ // file says Setion 2
				started=0;
				break;
			}
			else if(!strcmp(line,"Section-2")){
				started=1; 
				continue;
			}
			strcpy(word,line);
			normalise(word);
			temp=mapSearch(m,word);
			Notseen=1;
			for(;hasNext(addressbook[temp]->address);listNext(addressbook[temp]->address)){
				if(strcmp((char*)readList(addressbook[temp]->address),(char*)readList(urls))==0){
					Notseen=0;
					break;
				}
			}
			listReset(addressbook[temp]->address);
			if(Notseen){
				listEnter(addressbook[temp]->address,(char*)readList(urls));	
			}
		}		
		fclose(fp2);
		listNext(urls);
	}

	listReset(vocab);
	listReset(urls);
	numWord=listLength(vocab);

	mergesort((void**)addressbook, numWord, urlComp, 0);

	for(i=0;i<numWord;i++){
		fprintf(open,"%s ",addressbook[i]->name);
		for(;hasNext(addressbook[i]->address);listNext(addressbook[i]->address)) {
			fprintf(open,"%s ",(char*)readList(addressbook[i]->address));
		}
		listReset(addressbook[i]->address);
		fprintf(open,"\n");
	}


	for(i=0;i<numWord;i++){
		freeList(addressbook[i]->address);
		free(addressbook[i]);
	}
	free(addressbook);
	dropMap(m);
	freeList(vocab);
	freeList(urls);
	free(line);
	fclose(fp);
	fclose(open);
	return 0;
}	

