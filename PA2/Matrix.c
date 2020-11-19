//----------------------------------------------
// Matrix.c
// Dmitriy Nikitin ID:1649206 PA2
//----------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"Matrix.h"
#include"List.h"

typedef struct entryObj{
	int column;
	double value;
} entryObj;

typedef entryObj* Entry;

void freeEntry(Entry* pN){
	if(pN!=NULL && *pN!=NULL){
		free(*pN);
		*pN = NULL;
	}
}

Entry newEntry(int c, double v){
	Entry e = malloc(sizeof(entryObj));
	e->column = c;
	e->value = v;
	return e;
}

typedef struct MatrixObj{
	int NNZ;
	int size;
	List* row;
} MatrixObj;

typedef MatrixObj* Matrix;

double dotList(List A, List B);

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n){
	Matrix m = malloc(sizeof(MatrixObj));
	m->row = calloc(n, sizeof(List));
	for(int i = 0; i<n; i++){
		m->row[i] = newList();
	}
	m->size = n;
	m->NNZ = 0;
	return m;
}
// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
	if(pM!=NULL && *pM!=NULL){
		makeZero(*pM);
		for(int i = 0; i<size(*pM); i++){
			List l = (*pM)->row[i];
			freeList(&l);
			l = NULL;
		}
		free((*pM)->row);
		free(*pM);
		*pM = NULL;
	}
}
// Access functions
// size()
// Return the size of square Matrix M.
int size(Matrix M){
	return M->size;
}
// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M){
	return M->NNZ;
}
// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){
	if(size(A) != size(B)) return 0;
	else{
		for(int i = 0; i < size(A); i++){
			List r1 = A->row[i];
			List r2 = B->row[i];
			moveFront(r1);
			moveFront(r2);
			while(index(r1) != -1 && index(r2) != -1){
				if(length(r1) != length(r2)) return 0;
				if(((Entry)get(r1))->value != ((Entry)get(r2))->value) return 0;
				if(((Entry)get(r1))->column != ((Entry)get(r2))->column) return 0;
				moveNext(r1);
				moveNext(r2);
			}
		}
	}
	return 1;
}
// Manipulation procedures
// makeZero()
// Re-sets M to the zero Matrix.
void makeZero(Matrix M){
	for(int i = 0; i < size(M); i++){
		List l = M->row[i];
		moveFront(l);
		while(index(l) != -1){
			Entry e = ((Entry)get(l));
			freeEntry(&e);
			moveNext(l);
		}
		clear(l);
	}
	M->NNZ = 0;
}

//helper fort change entry to see if the column is exists in the row
int findCol(List l, int j){
	moveFront(l);
	while(index(l) != -1){
		if(((Entry)get(l))->column == j) return 1;
		moveNext(l);
	}
	return 0;
}


// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
	if(i > size(M) || i < 1 || j > size(M) || j < 1){
		exit(1);
	}
	int inserted = 0;
	 List l = M->row[i-1];
	 int colExists = findCol(l,j);
	 moveFront(l);
	 if(index(l) == -1 && x != 0){
	 	append(l, newEntry(j,x));
	 	M->NNZ++;
	 	inserted = 1;
	 }
	 else{
		 while(index(l)!=-1){
			if(((Entry)(get(l)))->column == j && x == 0.0){//case 2
				Entry e = ((Entry)(get(l)));
				M->NNZ--;
				delete(l);
				freeEntry(&e);
				inserted = 1;
				break;
			}
			else if(((Entry)(get(l)))->column > j && x != 0.0 && colExists == 0){//case 3
				insertBefore(l, newEntry(j,x));
				M->NNZ++;
				inserted = 1;
				break;
			}
			else if(((Entry)(get(l)))->column == j && x != 0.0){//case 4
				((Entry)(get(l)))->value = x;
				inserted = 1;
				break;
			}
			moveNext(l);
		}	
	}
	if(index(l) == -1 && inserted == 0 && x != 0){
		append(l,newEntry(j,x));
		M->NNZ++;
	}
	l = NULL;
}
// Matrix Arithmetic operations
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
	Matrix n = newMatrix(size(A));
	for(int i = 0; i<size(A); i++){
		//List l = newList();
		List a = A->row[i];
		moveFront(a);
		while(index(a) != -1){    
			Entry e = newEntry(((Entry)get(a))->column, ((Entry)get(a))->value);
			append(n->row[i],e);
			n->NNZ++;
			moveNext(a);
		}
	}
	return n;
}
// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A){
	Matrix n = newMatrix(size(A));
	for(int i = 0; i < size(A); i++){
		List l = A->row[i];
		moveFront(l);
		while(index(l) != -1){
			Entry oldE = (Entry)(get(l));
			int row = oldE->column - 1;
			int col = i+1;
			Entry newE = newEntry(col, oldE->value);
			append(n->row[row], newE);
			n->NNZ++;
			moveNext(l);
		}
	}
	return n;
 }
// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
	Matrix n = newMatrix(size(A));
	for(int i = 0; i<size(A); i++){
		List a = A->row[i];
		moveFront(a);
		while(index(a) != -1){
			Entry e = newEntry(((Entry)(get(a)))->column, (((Entry)(get(a)))->value) * x );
			append(n->row[i],e);
			n->NNZ++;
			moveNext(a);
		}
	}
	return n;
}
// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
	if(size(A) != size(B)){
		exit(1);
	}
	if(A==B){
		Matrix n = scalarMult(2.0, A);
		return n;
	}
	Matrix n = newMatrix(size(A));
	for(int i = 0; i<size(A); i++){
		List a = A->row[i];
		List b = B->row[i];
		moveFront(a);
		moveFront(b);
		//ADD CONDITONS ON IF ONE LIST IS SAMLLER THAN THE OTHER
		while(index(a) != -1 && index(b) != -1){
			if(((Entry)(get(a)))->column == ((Entry)(get(b)))->column){
			Entry e = newEntry(((Entry)(get(a)))->column, ((Entry)(get(a)))->value + ((Entry)(get(b)))->value);
				if(e->value != 0.0) {
					append(n->row[i],e);
					n->NNZ++;
					e = NULL;
				}
				else{
					freeEntry(&e);
				}
				moveNext(a);
				moveNext(b);
			}
			else if(((Entry)(get(a)))->column > ((Entry)(get(b)))->column){
				Entry e = newEntry(((Entry)(get(b)))->column, ((Entry)(get(b)))->value);
				if(e->value != 0.0) {
					append(n->row[i],e);
					n->NNZ++;
					e = NULL;
				}
				else{
					freeEntry(&e);
				}
				moveNext(b);
			}
			else/*if a column < b column*/{
			Entry e = newEntry(((Entry)(get(a)))->column, ((Entry)(get(a)))->value);
				if(e->value != 0.0) {
					append(n->row[i],e);
					n->NNZ++;
					e = NULL;
				}
				else{
					freeEntry(&e);
				}
				moveNext(a);
			}	
			
		}
		//if a runs out of elements add the rest of b onto the 0 elements of a
		if(index(b) != -1){
			while(index(b) != -1){
				append(n->row[i],newEntry(((Entry)(get(b)))->column, ((Entry)(get(b)))->value));
				n->NNZ++;
				moveNext(b);
			}
		}
		//vise versa
		if(index(a) != -1){
			while(index(a) != -1){
				append(n->row[i],newEntry(((Entry)(get(a)))->column, ((Entry)(get(a)))->value));
				n->NNZ++;
				moveNext(a);
			}
		}
	}
	return n;
}
// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
	if(size(A) != size(B)){
		exit(1);
	}
	Matrix n = newMatrix(size(A));
	if(A==B){
		return n;
	}
	for(int i = 0; i<size(A); i++){
		List a = A->row[i];
		List b = B->row[i];
		moveFront(a);
		moveFront(b);
		//ADD CONDITONS ON IF ONE LIST IS SAMLLER THAN THE OTHER
		while(index(a) != -1 && index(b) != -1){
			if(((Entry)(get(a)))->column == ((Entry)(get(b)))->column){
				Entry e = newEntry(((Entry)(get(a)))->column, ((Entry)(get(a)))->value - ((Entry)(get(b)))->value);
				if(e->value != 0.0) {
					append(n->row[i],e);
					n->NNZ++;
					e = NULL;
				}
				else{
					freeEntry(&e);
				}
				moveNext(a);
				moveNext(b);
			}
			else if(((Entry)(get(a)))->column > ((Entry)(get(b)))->column){
				Entry e = newEntry(((Entry)(get(b)))->column, 0.0 - ((Entry)(get(b)))->value);
				if(e->value != 0.0) {
					append(n->row[i],e);
					n->NNZ++;
					e = NULL;
				}
				else{
					freeEntry(&e);
				}
				moveNext(b);
			}
			else/*if a column < b column*/{
				Entry e = newEntry(((Entry)(get(a)))->column, ((Entry)(get(a)))->value);
				if(e->value != 0.0) {
					append(n->row[i],e);
					n->NNZ++;
					e = NULL;
				}
				else{
					freeEntry(&e);
				}
				moveNext(a);
			}
		}
		//if a runs out of elements subtract the rest of b from the 0 elements of a
		if(index(b) != -1){
			while(index(b) != -1){
				Entry e = newEntry(((Entry)(get(b)))->column, 0.0 - ((Entry)(get(b)))->value);
				if(e->value != 0.0) {
					append(n->row[i],e);
					n->NNZ++;
					e = NULL;
				}
				else{
					freeEntry(&e);
				}
				moveNext(b);
			}
		}
		//vise versa
		if(index(a) != -1){
			while(index(a) != -1){
				Entry e = newEntry(((Entry)(get(a)))->column, ((Entry)(get(a)))->value);
				if(e->value != 0.0) {
					append(n->row[i],e);
					n->NNZ++;
					e = NULL;
				}
				else{
					freeEntry(&e);
				}
				moveNext(a);
			}
		}
	}
	return n;
}
// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
	if(size(A) != size(B)){
		exit(1);
	}
	Matrix b = transpose(B);
	//printf("transpose of B done\n");

	Matrix n = newMatrix(size(A));
	for(int i = 0; i < size(A); i++){
		if(length(A->row[i]) != 0 || length(b->row[i]) != 0){
			for(int k = 0; k < size(b); k++){
				//printf("processing row %d, col %d\n", i, k);
				double value = dotList(A->row[i],b->row[k]);
				if(value != 0.0) {
					append(n->row[i], newEntry(k+1, value));
					n->NNZ++;
				}
			}
		}
	}
	freeMatrix(&b);
	return n;
}
// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M){
	for(int i = 0; i < size(M); i++){
		List l = M->row[i];
		if(length(l) != 0){	
			fprintf(out,"%d: ", i+1);
			moveFront(l);
			while(index(l) != -1){
				Entry e = ((Entry)get(l));
				fprintf(out, "(%d, %.1f) ", e->column, e->value);
				moveNext(l);
			}
			fprintf(out, "\n");
		}
	}
}

//helper functions
//takes the dot product of the two vectors
double dotList(List A, List B){
	double sum = 0; 
	moveFront(A);
	moveFront(B);
	while(index(A) != -1 && index(B) != -1){
		if(((Entry)(get(A)))->column == (((Entry)(get(B)))->column)){
			sum += (((Entry)(get(A)))->value) * (((Entry)(get(B)))->value);
			moveNext(A);
			moveNext(B);
		}
		else if((((Entry)(get(A)))->column) > (((Entry)(get(B)))->column)){
			moveNext(B);
		}
		else moveNext(A);
	}
	return sum;
}