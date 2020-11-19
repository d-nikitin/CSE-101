//-----------------------------------------------------------------------------
// BigInteger.c
// Dmitriy Nikitin ID:1649206 PA3
//-----------------------------------------------------------------------------

#include "BigInteger.h"
#include "List.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#define POWER 00
#define BASE 01

// Exported type -------------------------------------------------------------
// BigInteger reference type
typedef struct BigIntegerObj{
	int sign;
	List nums;
} BigIntegerObj;

typedef struct BigIntegerObj* BigInteger;

// Constructors-Destructors ---------------------------------------------------
// newBigInteger()
// Returns a reference to a new BigInteger object in the zero state.
BigInteger newBigInteger(){
	BigInteger a = malloc(sizeof(BigIntegerObj));
	assert(a!=NULL);
	a->sign = 0;
	a->nums = newList();
	return a;
}

// freeBigInteger()
// Frees heap memory associated with *pN, sets *pN to NULL.
void freeBigInteger(BigInteger* pN){
	if(pN != NULL && (*pN) != NULL){
		List l = (*pN)->nums;
		freeList(&l);
		l=NULL;
		free(*pN);
		*pN = NULL;
	}
}

// Access functions -----------------------------------------------------------
// sign()
// Returns -1 if N is negative, 1 if N is positive, and 0 if N is in the zero
// state.
int sign(BigInteger N){
	return N->sign;
}

// compare()
// Returns -1 if A<B, 1 if A>B, and 0 if A=B.
int compare(BigInteger A, BigInteger B){
	if(equals(A,B) == 1) return 0;
	else{
		List a = A->nums;
		List b = B->nums;
		if(length(a) < length(b)) return - 1;
		else if(length(a) > length(b)) return 1;
		else{//length(a) == length(b)
			moveFront(a);
			moveFront(b);
			while(index(a)!=-1){
				if(get(a)>get(b)) return 1;
				else if(get(a)<get(b)) return -1;
				else{//they are equal
					moveNext(a);
					moveNext(b);
				}
			}
			return 0;
		}
	}
}

// equals()
// Return true (1) if A and B are equal, false (0) otherwise.
int equals(BigInteger A, BigInteger B){
	if(sign(A) != sign(B)) return 0;
	List a = A->nums;
	List b = B->nums;
	if(length(a) != length(b)) return 0;
	moveFront(a);
	moveFront(b);
	while(index(a) != -1 && index(b) != -1){
		if(get(a) != get(b)) return 0;
		moveNext(a);
		moveNext(b);
	}
	return 1;
}

// Manipulation procedures ----------------------------------------------------
// makeZero()
// Re-sets N to the zero state.
void makeZero(BigInteger N){
	N->sign = 0;
	clear(N->nums);
}
// negate()
// Reverses the sign of N: positive <--> negative. Does nothing if N is in the
// zero state.
void negate(BigInteger N){
	if(N->sign != 0){
		N->sign = N->sign *-1;
	}
}

int isEmpty(BigInteger N){
	List l = N->nums;
	if(length(l) == 0) return 1;
	else return 0;
}

// BigInteger Arithmetic operations -----------------------------------------------
// stringToBigInteger()
// Returns a reference to a new BigInteger object representing the decimal integer
// represented in base 10 by the string s.
// Pre: s is a non-empty string containing only base ten digits {0,1,2,3,4,5,6,7,8,9}
// and an optional sign {+, -} prefix.
BigInteger stringToBigInteger(char* s){
	BigInteger new = newBigInteger();
	if(s[0] == '-'){
		new->sign = -1;
	}
	else {
		new->sign = 1;
	}
	long mult = 1;
	long value = 0;
	int size = strlen(s);
	for(int i = 1; i <= size; i++){
		if(s[size -i] != '+' && s[size -i] != '-'){
			value += (s[size - i] - '0') * mult;
			//mult *= 10;
			//if(i % POWER == 0){
				prepend(new->nums, value);
				//mult = 1;
				value = 0;
			//}
			//else if(i == size && value > 0) prepend(new->nums,value);
		}
		else if(i == size && value > 0) prepend(new->nums,value);
	}
	List l = new->nums;
	moveFront(l);
	while(index(l) != -1){
		if(get(l) == 0)delete(l);
		else break;
	}
	if(isEmpty(new) == 1) new->sign = 0;
	return new;
}

// copy()
// Returns a reference to a new BigInteger object in the same state as N.
BigInteger copy(BigInteger N){
	BigInteger new = newBigInteger();
	new->sign = N->sign;
	List l = N->nums;
	moveFront(l);
	while(index(l) != -1){
		append(new->nums, get(l));
		moveNext(l);
	}
	return new;
}

//Helper functions
//for addition and subtraction methods 
void normalize(BigInteger A){
	int carry = 0;
	List l = A->nums;
	moveFront(l);
	if(get(l)<0){
		while(index(l)!=-1){
			set(l,get(l)*-1);
			moveNext(l);
		}
	}
	moveBack(l);
	while(index(l) != -1){
		set(l, get(l) + carry);
		carry = 0;
		if(get(l) >= BASE){
			carry = get(l)/BASE;
			//set(l, get(l)%BASE);
			set(l, get(l)-carry+1);
		}
		else if(get(l) < 0){
			if(index(l) == 0){}
			else{
				carry = -1;
				set(l, get(l) + BASE);
			}
		}
	movePrev(l);	
	}
	if(carry != 0)prepend(l,carry);
	moveFront(l);
	if(get(l) < 0){
		A->sign = -1;
		set(l,get(l) * -1);
	}
	else if(get(l) > 0){
		A->sign = 1;
	}
}

// add()
// Places the sum of A and B in the existing BigInteger S, overwriting its
// current state: S = A + B
void add(BigInteger S, BigInteger A, BigInteger B){
	makeZero(S);
	if(A==B){
		BigInteger C = copy(B);
		add(S,A,C);
		freeBigInteger(&C);
	}
	else{
		if(A->sign == -1 && B->sign == 1){
			negate(A);
			subtract(S,B,A);
			negate(A);
		}
		else if(B->sign == -1 && A->sign == 1){
			negate(B);
			subtract(S,A,B);
			negate(B);
		}
		else{// both positive or both negative 
			List a = A->nums;
			List b = B->nums;
			moveBack(a);
			moveBack(b);
			List temp = S->nums;
			while(index(a) != -1 && index(b) != -1){
				prepend(temp, get(a) + get(b));
				movePrev(a);
				movePrev(b);
			}
			// if a is longer than b
			while(index(a) != -1){
				prepend(temp, get(a));
				movePrev(a);
			}
			//vise versa
			while(index(b) != -1){
				prepend(temp, get(b));
				movePrev(b);
			}
			normalize(S);
			moveFront(temp);
			if(get(temp) == 0) S->sign = 0;
			else if(A->sign == -1 && B->sign == -1) S->sign = -1;
			else S->sign = 1;

		}
	}
}
// sum()
// Returns a reference to a new BigInteger object representing A + B.
BigInteger sum(BigInteger A, BigInteger B){
	BigInteger S = newBigInteger();
	if(A==B) {
		BigInteger C = copy(B);
		add(S,A,C);
		freeBigInteger(&C);
	}
	else add(S,A,B);
	return S;

}
// subtract()
// Places the difference of A and B in the existing BigInteger D, overwriting
// its current state: D = A - B
void subtract(BigInteger D, BigInteger A, BigInteger B){
	makeZero(D);
	if(A==B){
		BigInteger C = copy(B);
		subtract(D,A,C);
		freeBigInteger(&C);
	}
	else{
		if(A->sign == -1 && B->sign == -1){
			negate(B);
			add(D,A,B);
			negate(B);
		}
		else if(A->sign == -1 && B->sign == 1){
			negate(B);
			add(D,A,B);
			negate(B);
		}
		else if(A->sign == 1 && B->sign == -1){
			negate(B);
			add(D,A,B);
			negate(B);
		}
		else{//both positive
			List a = A->nums;
			List b = B->nums;
			moveBack(a);
			moveBack(b);
			List temp = D->nums;
			while(index(a) != -1 && index(b) != -1){
				prepend(temp, get(a) - get(b));
				movePrev(a);
				movePrev(b);
			}
			// if a is longer than b
			while(index(a) != -1){
				prepend(temp, get(a));
				movePrev(a);
			}
			//vise versa
			while(index(b) != -1){
				prepend(temp, get(b)  * -1);
				movePrev(b);
			}
			normalize(D);
			moveFront(temp);
			D->sign = compare(A,B);
		}
	}
}
// diff()
// Returns a reference to a new BigInteger object representing A - B.
BigInteger diff(BigInteger A, BigInteger B){
	BigInteger S = newBigInteger();
	if(A==B){
		BigInteger C = copy(B);
		subtract(S,A,C);
		freeBigInteger(&C);
		S->sign = 0;
		clear(S->nums);
	}
	else subtract(S,A,B);
	return S;
}
//adds the temp int to the final int without overwriting the entries in final
void sumMult(BigInteger final, BigInteger temp){
	List f = final->nums;
	List t = temp->nums;
	moveBack(f);
	moveBack(t);
	long sum = 0;
	while(index(f)!=-1 && index(t)!=-1){
		sum = get(f) + get(t);
		set(f,sum);
		movePrev(f);
		movePrev(t);
	}
	while(index(t)!=-1){
		prepend(f,get(t));
		movePrev(t);
	}
	normalize(final);
}

// multiply()
// Places the product of A and B in the existing BigInteger P, overwriting
// its current state: P = A*B
void multiply(BigInteger P, BigInteger A, BigInteger B){
	makeZero(P);
	if(A==B){
		BigInteger C = copy(B);
		multiply(P,A,C);
		freeBigInteger(&C);
	}
	else{
		BigInteger final = newBigInteger();
		BigInteger temp = newBigInteger(); 
		List a = A->nums;
		List b = B->nums;
		List t = temp->nums;
		moveBack(b);
		int counter = 0;
		while(index(b)!=-1){
			moveBack(a);
			clear(t);
			for(int j = 0; j < counter; j++)
				prepend(t,0);
			while(index(a)!=-1){
				prepend(t, get(a) * get(b));
				movePrev(a);
			}
			normalize(temp);
			sumMult(final,temp);
			movePrev(b);
			counter++;
		}
		moveFront(final->nums);
		while(index(final->nums) != -1){
			append(P->nums,get(final->nums));
			moveNext(final->nums);
		}
		freeBigInteger(&final);
		freeBigInteger(&temp);
		if(A->sign == -1 && B->sign == 1){
			P->sign = -1;
		}
		else if(A->sign == 1 && B->sign == -1){
			P->sign = -1;
		}
		else{
			P->sign = 1;
		}
	}
}
// prod()
// Returns a reference to a new BigInteger object representing A*B
BigInteger prod(BigInteger A, BigInteger B){
	BigInteger P = newBigInteger();
	if(A==B){
		BigInteger C = copy(B);
		multiply(P,A,C);
		freeBigInteger(&C);
	}
	else multiply(P,A,B);
	return P;
}
// Other operations -----------------------------------------------------------
// printBigInteger()
// Prints a base 10 string representation of N to filestream out.
void printBigInteger(FILE* out, BigInteger N){
	List temp = N->nums;
	int front = 1;
	moveFront(temp);
	if(isEmpty(N) == 1){
		fprintf(out, "0");
	}
	else{
		if(N->sign == -1) fprintf(out, "%c", '-');
		while(index(temp) != -1){
			if(front == 1){
				if(get(temp) == 0){}
				else{
					fprintf(out, "%ld", get(temp));
					front = 0;
				}
			}
			else fprintf(out, "%09ld", get(temp));
			moveNext(temp);
		}
	}
	fprintf(out,"\n");
}

//for my sanity as I debug
void printBigIntegerTEST(FILE* out, BigInteger N){
	if(N->sign == 0){
		fprintf(out, "0"); 
	}
	else{
		List temp = N->nums;
		moveFront(temp);
		if(N->sign == -1) fprintf(out, "%c", '-');
		while(index(temp) != -1){
			if(index(temp) == 0 && get(temp) != 0) fprintf(out, "%ld ", get(temp));
			else fprintf(out, "%02ld ", get(temp));
			moveNext(temp);
		}
	}
	fprintf(out,"\n");
}