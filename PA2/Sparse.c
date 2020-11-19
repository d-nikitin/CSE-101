//-----------------------------------------------------------------------------
// Sparse.c
// Dmitriy Nikitin ID:1649206 PA2
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 200
#include "Matrix.h"

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

  	int size = 0; 
  	int NNZ1 = 0;
  	int NNZ2 = 0;

  	if(fgets(line, MAX_LEN, in) == NULL) {
  		fprintf(stderr, "Invalid input file format can not read first line");
  		exit(1);
  	}
  	
  	int n = sscanf(line,"%d %d %d", &size, &NNZ1, &NNZ2);

  	if(n != 3){
  		fprintf(stderr, "Invalid input file format, first line should contain 3 ints");
  		exit(2);
  	}

  	//printf("size=%d M1=%d M2=%d\n", size, NNZ1, NNZ2);

  	fgets(line, MAX_LEN, in); // ignore first empty line after header

  	int row = 0, col = 0;
  	double val = 0.0;

  	Matrix A = newMatrix(size);

  	for(int i = 0; i<NNZ1; i++){
  		if(fgets(line, MAX_LEN, in) == NULL){
  			fprintf(stderr, "Invalid input file format, unexcpetected EOF");
  			exit(3);
  		}
  		//printf("line = %s\n", line);
  		row = col = 0; // clean previous values just in case
  		val = 0.0;
  		n = sscanf(line,"%d %d %lf", &row, &col, &val);
  		if(n != 3){
  			fprintf(stderr, "Invalid input file format, every data line should contain 2 integers and double");
  			exit(4);
  		}
  		changeEntry(A, row, col, val);
  	}

  	fgets(line, MAX_LEN, in); // ignore second empty line after first matrix data

  	Matrix B = newMatrix(size);

  	for(int i = 0; i<NNZ2; i++){
  		if(fgets(line, MAX_LEN, in) == NULL){
  			fprintf(stderr, "Invalid input file format, unexcpetected EOF");
  			exit(3);
  		}
  		//printf("line = %s\n", line);
  		row = col = 0; // clean previous values just in case
  		val = 0.0;
  		n = sscanf(line,"%d %d %lf", &row, &col, &val);
  		if(n != 3){
  			fprintf(stderr, "Invalid input file format, every data line should contain 2 integers and double");
  			exit(4);
  		}
  		changeEntry(B, row, col, val);
  	}

  	fclose(in);

  	//printf("input read\n");

  	fprintf(out,"A has %d non-zero entries:\n", NNZ1);
 	printMatrix(out, A);
 	fprintf(out, "\n");
  	//printf("Matrix 1 printed\n");

  	fprintf(out,"B has %d non-zero entries:\n", NNZ2);
  	printMatrix(out, B);
  	fprintf(out, "\n");
  	//printf("Matrix 2 printed\n");

	Matrix c;
  	fprintf(out, "(1.5)*A =\n");
  	c = scalarMult(1.5,A);
  	printMatrix(out, c);
  	fprintf(out, "\n");
  	if( c != NULL ) freeMatrix(&c);
 	//printf("Matrix 1.5*A printed\n");

  	fprintf(out, "A+B =\n");
  	c = sum(A,B);
  	printMatrix(out, c);
  	fprintf(out, "\n");
  	if(c != NULL) freeMatrix(&c);
 	//printf("Matrix A+B printed\n");

  	fprintf(out, "A+A =\n");
  	c = sum(A,A);
  	printMatrix(out, c);
  	fprintf(out, "\n");
  	if(c != NULL) freeMatrix(&c);
 	//printf("Matrix A+A printed\n");

  	fprintf(out, "B-A =\n");
  	c = diff(B,A);
  	printMatrix(out, c);
  	fprintf(out, "\n");
  	if(c != NULL) freeMatrix(&c);
 	//printf("Matrix B-A printed\n");

  	fprintf(out, "A-A =\n");
  	c = diff(A,A);
  	printMatrix(out, c);
  	fprintf(out, "\n");
  	if(c != NULL) freeMatrix(&c);
 	//printf("Matrix A-A printed\n");

  	fprintf(out, "Transpose(A) =\n");
  	c = transpose(A);
  	printMatrix(out, c);
  	fprintf(out, "\n");
  	if(c != NULL) freeMatrix(&c);
 	//printf("Matrix Trans(A) printed\n");

  	fprintf(out, "A*B =\n");
  	c = product(A,B);
  	printMatrix(out, product(A,B));
  	fprintf(out, "\n");
  	if(c != NULL) freeMatrix(&c);
 	//printf("Matrix A*B printed\n");

	fprintf(out, "B*B =\n");
	c = product(B,B);
  	printMatrix(out, product(B,B));
  	fprintf(out, "\n");
  	if(c != NULL) freeMatrix(&c);
 	//printf("Matrix B*B printed\n");


 /*
  	fprintf(stdout,"B has %d non-zero entries:\n", NNZ2);
  	printMatrix(stdout, B);
  	printf("Matrix 2 printed\n");

  	fprintf(stdout, "Transpose(B) =\n");
  	printMatrix(stdout, transpose(B));
 	printf("Matrix Trans(B) printed\n");

  	fprintf(stdout,"B has %d non-zero e-ntries:\n", NNZ2);
 	printMatrix(stdout, B);
  	printf("Matrix 2 printed\n");

  	fprintf(stdout, "Transpose(B) =\n");
  	printMatrix(stdout, transpose(B));
 	printf("Matrix Trans(B) printed\n");

  	fprintf(stdout,"B has %d non-zero e-ntries:\n", NNZ2);
 	printMatrix(stdout, B);
  	printf("Matrix 2 printed\n");

	fprintf(stdout, "B*B =\n");
  	printMatrix(stdout, product(B,B));
 	printf("Matrix B*B printed\n");

  	fprintf(stdout,"B has %d non-zero entries:\n", NNZ2);
  	printMatrix(stdout, B);
  	printf("Matrix 2 printed\n");

	fprintf(stdout, "B*B =\n");
  	printMatrix(stdout, product(B,B));
 	printf("Matrix B*B printed\n");

  	fprintf(stdout,"B has %d non-zero entries:\n", NNZ2);
  	printMatrix(stdout, B);
  	printf("Matrix 2 printed\n");
*/
  	freeMatrix(&A);
  	freeMatrix(&B);
  	fflush(out);
  	fclose(out);

  	return (0);
}