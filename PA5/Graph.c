//----------------------------------------------
// Graph.c
// Dmitriy Nikitin ID:1649206 PA5
//----------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"Graph.h"
#include"List.h"
#define UNDEF -1
#define NIL 0

typedef struct GraphObj{
	List *adj;
	char *color;
	int *p;
	int *d;
	int *f;
	int size;
	int order;
}GraphObj;

typedef GraphObj* Graph;

/*** Constructors-Destructors ***/
Graph newGraph(int n){ //returns new graph
	Graph g = malloc(sizeof(GraphObj));
	g->adj = (List*) malloc((n+1)*sizeof(List));
	for(int i = 1; i <= n; i++){
		g->adj[i] = newList();
	}
	g->p = (int*) malloc((n+1)*sizeof(int));
	g->d = (int*) malloc((n+1)*sizeof(int));
	g->color = (char*) malloc((n+1)*sizeof(char));
	g->f = (int*) malloc((n+1)*sizeof(int));
	for(int i = 1; i <= n; i++){
		g->p[i] = NIL;
		g->d[i] = UNDEF;
		g->f[i] = UNDEF;
		g->color[i] = 'w';
	}
	g->size = 0;
	g->order = n;
	return (g);
}
void freeGraph(Graph* pG){ //frees all dynamic memory associated with Graph
	if(pG!=NULL && *pG!=NULL){
		List l;
		for(int i =1; i <= (*pG)->order; i++){
			l = (*pG)->adj[i];
			freeList(&l);
		}
		free((*pG)->adj);
		free((*pG)->p);
		free((*pG)->d);
		free((*pG)->f);
		free((*pG)->color);
		free(*pG);
	}
}
/* Access functions */
int getOrder(Graph G){
	return (G->order);
}
int getSize(Graph G){
	return (G->size);
}
int getParent(Graph G, int u){
	if(u>=1 && u <= getOrder(G))
		return (G->p[u]);
}
int getDiscover(Graph G, int u){
	if(u>=1 && u <= getOrder(G))
		return (G->d[u]);
}
int getFinish(Graph G, int u){
	if(u>=1 && u <= getOrder(G))
		return (G->f[u]);
}
/* Manipulation procedures */
void addEdge(Graph G, int u, int v){
	if(u!=0 && v!=0){
		List temp = G->adj[u];
		if(length(temp) != 0){
			moveFront(temp);
			while(index(temp)!=-1){
				if(get(temp)>v){
					insertBefore(temp,v);
					break;
				}
				else if(index(temp) == length(temp)-1) {
					append(G->adj[u],v);
					break;
				}
				moveNext(temp);
			}
		}
		else append(G->adj[u],v);
		temp = G->adj[v];
		if(length(temp) != 0){
			moveFront(temp);
			while(index(temp)!=-1){
				if(get(temp)>u){
					insertBefore(temp,u);
					break;
				}
				else if(index(temp) == length(temp)-1) {
					append(G->adj[v],u);
					break;
				}
				moveNext(temp);
			}
		}
		else append(G->adj[v],u);
		G->size += 1;
	}
}
void addArc(Graph G, int u, int v){
	if(u!=0 && v!=0){
		List temp = G->adj[u];
		if(length(temp) != 0){
			moveFront(temp);
			while(index(temp)!=-1){
				if(get(temp)>v){
					insertBefore(temp,v);
					break;
				}
				else if(index(temp) == length(temp)-1) {
					append(G->adj[u],v);
					break;
				}
				moveNext(temp);
			}
		}
		else append(G->adj[u],v);
		G->size += 1;
	}
}

void Visit(Graph G, List S, int* time, int x){
	(*time)++;
	G->d[x] = *time;
	G->color[x] = 'g';
	List l = G->adj[x];
	moveFront(l);
	int y;
	while(index(l)!=-1){
		y = get(l);
		if(G->color[y] == 'w'){
			G->p[y] = x;
			Visit(G,S,time,y);
		}
		moveNext(l);
	}
	G->color[x] = 'b';
	(*time)++;
	G->f[x] = *time;
	append(S,x);
}

void DFS(Graph G, List S){
	if(length(S) == getOrder(G)){
		for(int i = 1; i <= getOrder(G); i++){
			G->color[i] = 'w';
			G->p[i] = NIL;
		}
		int time = 0;
		int x;
		for(int i = 1; i <=getOrder(G); i++){
			x = front(S);
			deleteFront(S);
			if(G->color[x] == 'w') Visit(G,S,&time,x);
		}
		List temp = copyList(S);
	    clear(S);
	    moveBack(temp);
	    while(index(temp)!=-1){
	      append(S,get(temp));
	      movePrev(temp);
	    }
	    freeList(&temp);
	}
}


/*** Other operations ***/
Graph transpose(Graph G){
	Graph n = newGraph(getOrder(G));
	for(int i = 1; i <= getOrder(G); i++){
		List l = G->adj[i];
		moveFront(l);
		while(index(l)!=-1){
			int u = i;
			int v = get(l);
			addArc(n,v,u);
			moveNext(l);
		}
	}
	return n;
}

Graph copyGraph(Graph G){
	Graph n = newGraph(getOrder(G));
	for(int i = 1; i <= getOrder(G); i++){
		List l = G->adj[i];
		moveFront(l);
		while(index(l)!=-1){
			int u = i;
			int v = get(l);
			addArc(n,u,v);
			moveNext(l);
		}
	}
	return n;
}

void printGraph(FILE* out, Graph G){
	List temp;
	for(int i = 1; i<=getOrder(G); i++){
		temp = G->adj[i];
		moveFront(temp);
		fprintf(out, "%d: ", i);
		while(index(temp) != -1){
			if(index(temp) == length(temp) - 1) fprintf(out, "%d", get(temp));
			else fprintf(out, "%d ", get(temp));
			moveNext(temp);
		}
		fprintf(out, "\n");
	}
	temp = NULL;
}