//----------------------------------------------
// List.c
//  
//----------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"List.h"

typedef struct NodeObj{
	int val;
	struct NodeObj* left;
	struct NodeObj* right;
} NodeObj;

typedef NodeObj* Node;

Node newNode(int x){
	Node n = malloc(sizeof(NodeObj));
	n->val = x;
	n->left = NULL;
	n->right = NULL;
	return(n);
}

void freeNode(Node* pN){
	if(pN!=NULL && *pN!=NULL){
		free(*pN);
		*pN = NULL;
	}
}

typedef struct ListObj{
	Node front;
	Node back;
	Node cursor;
	int numitems;
	int index;
} ListObj;

typedef ListObj* List;

List newList(void){
	List l = malloc(sizeof(ListObj));
	l->front = NULL;
	l->back = NULL;
	l->cursor = NULL;
	l->index = -1;
	l->numitems = 0;
	return(l);
}

void freeList(List* pL){
	if(pL!=NULL && *pL!=NULL){
		clear(*pL);
		free(*pL);
		*pL = NULL;
	}
}

// Access functions -----------------------------------------------------------
int length(List L) {
	return L->numitems;
}
//Returns the number of elements in L.

int index(List L){
	if(L->cursor == NULL) return -1;
	else return L->index;
}
//Returns index of cursor element if defined, -1 otherwise.

int front(List L){
	if(L->numitems==0){
		fprintf(stderr, "List Error: calling front(List L) on empty List");
		exit(1);
	}
	return L->front->val;
} 
// Returns front element of L. Pre: length()>0

int back(List L){
	if(L->numitems==0){
		fprintf(stderr, "List Error: calling back(List L) on empty List");
		exit(1);
	}
	return L->back->val;
}
// Returns back element of L. Pre: length()>0

int get(List L){
	if(L->numitems==0){
		fprintf(stderr, "List Error: calling get(List L) on empty List");
		exit(1);
		}
		else return L->cursor->val;

} 
// Returns cursor element of L. Pre: length()>0, index()>=0
//*ATTENTION FIX THIS BULLSHIT*
int equals(List A, List B){
	if(length(A)==length(B)){
		Node a = A->front;
		Node b = B->front;
		while(a!=NULL && b!=NULL){
			if(a->val == b->val){
				a = a->right;
				b = b->right;
			}
			else return 0;
		}
		return 1;
	}
	else return 0;
} 
// Returns true (1) iff Lists A and B are in same
// state, and returns false (0) otherwise.

// Manipulation procedures ----------------------------------------------------
void clear(List L){
	if(length(L)!=0){
		while(L->numitems>0){
			deleteFront(L);
		}
		/*L->front = NULL;
		L->back = NULL;
		L->cursor = NULL;
		L->index = -1;
		L->numitems = 0;*/
	}
}
// Resets L to its original empty state.

void moveFront(List L){
	if(length(L)!=0){
		L->cursor = L->front;
		L->index = 0;
	}
}
// If L is non-empty, sets cursor under the front element,

// otherwise does nothing.
void moveBack(List L){
	if(length(L)!=0){
		L->cursor = L->back;
		L->index = L->numitems - 1;
	}
} 
// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.

void movePrev(List L){
	if(L->cursor!=NULL){
			L->cursor = L->cursor->left;
			L->index -= 1;
			if(L->cursor == NULL)L->index = -1;
	}
}
// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing

void moveNext(List L){
	if(L->cursor!=NULL){
		L->cursor = L->cursor->right;
		L->index +=1; 
		if(L->cursor == NULL) L->index = -1;
	}
} 
// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing

void prepend(List L, int data){
	Node n = newNode(data);
	if(L->numitems == 0){
		L->front = n;
		L->back = n;
	}
	else{
		n->right = L->front;
		L->front->left = n;
		L->front = n;
	}
	L->numitems += 1;
	if (L->cursor!=NULL)L->index += 1;
} 
// Insert new element into L. If L is non-empty,
// insertion takes place before front element.

void append(List L, int data){
	Node n = newNode(data);
	if(L->numitems == 0){
		L->front = n;
		L->back = n;
	}
	else if(L->numitems == 1){
		L->back = n;
		L->front->right = L->back;
		L->back->left = L->front;
	}
	else{
		n->left = L->back;
		L->back->right = n;
		L->back = n;
	}
	L->numitems += 1;
}
// Insert new element into L. If L is non-empty,
// insertion takes place after back element.

void insertBefore(List L, int data){
	if(length(L)>0 && L->index >= 0){	
		if(L->index == 0) prepend(L,data);
		else{
			Node n = newNode(data);
			Node temp = L->cursor->left;
			temp->right = n;
			n->left = temp;
			n->right = L->cursor;
			L->cursor->left = n;
			L->numitems += 1;
			L->index += 1;
			}
		}

} 
// Insert new element before cursor.
// Pre: length()>0, index()>=0

void insertAfter(List L, int data){
	if(length(L)>0 && L->index >= 0){
		if(L->index == L->numitems -1) append(L,data);
		else{
			Node n = newNode(data);
			Node temp = L->cursor->right;
			temp->left = n;
			n->right = temp;
			n->left = L->cursor;
			L->cursor->right = n;
			L->numitems += 1;
		}
	}
} 
// Insert new element after cursor.
// Pre: length()>0, index()>=0

void deleteFront(List L){
	if(length(L)>0){
		Node n = L->front;
		if(length(L)==1){
			L->front = NULL;
			L->back = NULL;
			L->cursor = NULL;
			L->index = -1;
		}
		else{
			L->front = L->front->right;
			L->front->left = NULL;
			L->index--;
			if(L->index==-1) L->cursor = NULL;
		}
		freeNode(&n);
		L->numitems -= 1;
	}
}
// Delete the front element. Pre: length()>0

void deleteBack(List L){
	if(length(L)>0){
		Node n = L->back;
		if(n!=NULL){
			if(n->left==NULL){
				freeNode(&n);
				L->numitems -= 1;
				L->back = NULL;
				L->front = NULL;
				L->index = -1;
			}
			else{
			Node temp = n->left;
			temp->right = NULL;
			L->back = temp;
		 	if(n == L->cursor) L->index = -1;
			freeNode(&n);
			L->numitems -= 1;
			}
		}
	}
} 
// Delete the back element. Pre: length()>0

void delete(List L){
	if(length(L)>0 && L->index >= 0){
		if(L->index == 0) deleteFront(L);
		else if(L->index == L->numitems - 1) deleteBack(L);
		else{
			Node n = L->cursor;
			L->cursor->left->right = L->cursor->right;
			L->cursor->right->left = L->cursor->left;
			L->cursor = NULL;
			L->index = -1;
			freeNode(&n);
			L->numitems -= 1;
		}
	}
} 
// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0

// Other operations -----------------------------------------------------------
void printList(FILE* out, List L){
	Node temp = L->front;
	while(temp!=NULL){
		fprintf(out, "%d ", temp->val);
		temp = temp->right;
	}
} // Prints to the file pointed to by out, a
 // string representation of L consisting
 // of a space separated sequence of integers,
// with front on left.
List copyList(List L){
	List k = newList();
	Node n = L->front;
	while(n != NULL){
		append(k, n->val);
		n = n->right;
	}
	return k;
} // Returns a new List representing the same integer
 // sequence as L. The cursor in the new list is undefined,
// regardless of the state of the cursor in L. The state
// of L is unchanged.