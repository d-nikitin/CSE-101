//-----------------------------------------------------------------------------
// Arithmetic.c
// Dmitriy Nikitin ID:1649206 PA3
//-----------------------------------------------------------------------------

#include "BigInteger.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LEN 160000

int main(int argc, char* argv[]){
	FILE *in, *out;
	char line[MAX_LEN];
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

	fgets(line,MAX_LEN,in);
	int x =0;
	sscanf(line, "%d", &x);
	fgets(line,MAX_LEN,in);
	if(line[0] == '-' || line[0] == '+') x++;
	char temp1[x+1];
	strcpy(temp1,line);
	temp1[x] = '\0';

	BigInteger A = stringToBigInteger(temp1);

	fgets(line,MAX_LEN,in);
	sscanf(line, "%d", &x);
	fgets(line,MAX_LEN,in);
	if(line[0] == '-' || line[0] == '+') x++;
	char temp2[x+1];
	strcpy(temp2,line);
	temp2[x] = '\0';

	fclose(in);
	BigInteger B = stringToBigInteger(temp2);
	
	//A
	printBigInteger(out,A);
	fprintf(out, "\n");

	//B
	printBigInteger(out,B);
	fprintf(out, "\n");

	//A+B
	BigInteger c = 	sum(A,B);
	printBigInteger(out,c);
	fprintf(out, "\n");
	freeBigInteger(&c);

	//A-B
	BigInteger d = diff(A,B);
	printBigInteger(out,d);
	fprintf(out, "\n");
	freeBigInteger(&d);

	//A-A
	BigInteger e = diff(A,A);
	printBigInteger(out,e);
	fprintf(out, "\n");
	freeBigInteger(&e);

	//3A-2B
	BigInteger f = sum(A,A);
	BigInteger F = sum(f,A);
	BigInteger G = sum(B,B);
	subtract(f,F,G);
	printBigInteger(out,f);
	fprintf(out, "\n");
	freeBigInteger(&f);
	freeBigInteger(&F);
	freeBigInteger(&G);

	//AB
	BigInteger g = prod(A,B);
	printBigInteger(out,g);
	fprintf(out, "\n");
	freeBigInteger(&g);

	//A*A
	BigInteger j = prod(A,A);
	printBigInteger(out,j);
	fprintf(out, "\n");

	//B*B
	BigInteger k = prod(B,B);
	printBigInteger(out,k);
	fprintf(out, "\n");

	//9A^4 + 16B^5
	BigInteger l = prod(j,j);//A^4
	BigInteger a1 = sum(l,l);//2
	BigInteger a2 = sum(a1,a1);//4
	BigInteger a3 = sum(a2,a2);//8
	BigInteger a = sum(a3,l);//9

	BigInteger p = prod(k,k);//B^4
	BigInteger y = prod(p,B);//B^5

	BigInteger b1 = sum(y,y);//2
	BigInteger b2 = sum(b1,b1);//4
	BigInteger b3 = sum(b2,b2);//8
	BigInteger b = sum(b3,b3);//16

	add(l,a,b);
	printBigInteger(out,l);
	fprintf(out, "\n");

	//free all the BigInts
	freeBigInteger(&k);
	freeBigInteger(&j);
	freeBigInteger(&l);
	freeBigInteger(&a1);
	freeBigInteger(&a2);
	freeBigInteger(&a3);
	freeBigInteger(&a);
	freeBigInteger(&b1);
	freeBigInteger(&b2);
	freeBigInteger(&b3);
	freeBigInteger(&b);
	freeBigInteger(&p);
	freeBigInteger(&y);
	freeBigInteger(&A);
	freeBigInteger(&B);

	fflush(out);
	fclose(out);
	return 0;
}