//-----------------------------------------------------------------------------
// BigIntegerTest.c
// Dmitriy Nikitin ID:1649206 PA3
//-----------------------------------------------------------------------------
#include "BigInteger.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){
	char one[] = {"-0041085449"};
	char two[] = {"7402779357"};

	BigInteger A = stringToBigInteger(one);
	BigInteger B = stringToBigInteger(two);

	printBigIntegerTEST(stdout, A);
	printBigIntegerTEST(stdout, B);
	//A+B
	BigInteger c = sum(A,B);

	printBigIntegerTEST(stdout, c);

	//A-B
	BigInteger d = diff(A,B);

	printBigIntegerTEST(stdout, d);

	//A-A
	BigInteger e = diff(A,A);
	printBigIntegerTEST(stdout,e);
	freeBigInteger(&e);

	//3A-2B
	BigInteger f = sum(A,A);
	BigInteger F = sum(f,A);
	BigInteger G = sum(B,B);
	subtract(f,F,G);
	printBigIntegerTEST(stdout,f);
	freeBigInteger(&f);
	freeBigInteger(&F);
	freeBigInteger(&G);

	//AB
	BigInteger g = prod(A,B);
	printBigIntegerTEST(stdout,g);
	freeBigInteger(&g);

	//A*A
	BigInteger j = prod(A,A);
	printBigIntegerTEST(stdout,j);

	//B*B
	BigInteger k = prod(B,B);
	printBigIntegerTEST(stdout,k);

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
	printBigIntegerTEST(stdout,l);

	return 0;
}

/*
-41085449

7402779357

7361693908

-7443864806

0

-14928815061

-30416513730276293

1688014119531601

54801142208425333449

355708308072979699484782930346148190803050903225721
*/