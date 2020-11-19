//-------------------------------------------------------------
// Lex.c
// Dmitriy Nikitin ID:1649206
// PA1
//-------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

#define MAX_LEN 160

int main(int argc, char* argv[]){
	FILE *in, *out;
	fprintf(stdout, "2");
	char line[MAX_LEN];
	List l = newList();
    int numLines = 0;

	if( argc != 3 ){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

	// open files for reading and writing 
   in = fopen(argv[1], "r");
   out = fopen(argv[2], "w");

   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }

   if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
  	}


	while(fgets(line, MAX_LEN, in) != NULL){
		numLines++;
	}

	fprintf(stdout, "%d", numLines);

	char words[numLines][MAX_LEN +1];

	fclose(in);
	in = fopen(argv[1], "r");

    //puts words into array for easier handling
	for(int x=0; x<numLines; x++){
		fgets(line, MAX_LEN, in);
		int len = strlen(line);
		if(len>0){
			if(line[len-1]=='\n'){
				line[len-1] = '\0';
			}
		}
		strcpy(words[x],line);
	}

	fclose(in);
	append(l,0);

	//Insertion sort
	for(int i = 1; i < numLines; i++){
		moveFront(l);
		int j = i -1;
		char* temp = words[i];
		while(index(l)!=j){
			moveNext(l);
		}
		while(j>=0 && strcmp(temp,words[get(l)])<0){
			movePrev(l);
			j--;
		}
		if(index(l) <= -1) prepend(l,i);
		else insertAfter(l,i);
	}

	//prints in order
	moveFront(l);
	for(int k = 0; k < numLines; k++){
		int f = get(l);
		moveNext(l);
		fprintf(out,"%s\n", words[f]);
	}

	freeList(&l);
	fclose(out);
	return(0);
}

/*
int sort(int* a, n){
	for(int i = 1; i < n; i++){
		int temp = a[i];
		for(int j = i -1; j >= 0 && A[j] > A[i]; j--){
			a[j+1] = a[j];
		}
		a[j+1] = a[j];
		a[j] = temp;
	}
}*/