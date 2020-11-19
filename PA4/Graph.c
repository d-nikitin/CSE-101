//----------------------------------------------
// Graph.c
// Dmitriy Nikitin ID:1649206 PA4
//----------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"Graph.h"
#include"List.h"
#define INF -1
#define NIL -99

typedef struct GraphObj{
	List *adj;
	char *color;
	int *p;
	int *d;
	int size;
	int order;
	int source;
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
	for(int i = 1; i <= n; i++){
		g->p[i] = NIL;
		g->d[i] = INF;
		g->color[i] = 'w';
	}
	g->size = 0;
	g->order = n;
	g->source = NIL;
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
		free((*pG)->color);
		free(*pG);
	}
}
/*** Access functions ***/
int getOrder(Graph G){
	return (G->order);
}
int getSize(Graph G){
	return (G->size);
}
int getSource(Graph G){
	return (G->source);
}
int getParent(Graph G, int u){
	return (G->p[u]);
}
int getDist(Graph G, int u){
	return (G->d[u]);
}
void getPath(List L, Graph G, int u){
	/*if(getSource(G)!=NIL){
		int t = u;
		clear(L);
		while(G->p[t]!= NIL){
			prepend(L,t);
			t = G->p[t];
		}
		if(t!=getSource(G)){
			clear(L);
			prepend(L,NIL);
		}
		else prepend(L,t);
	}*/
	if(getDist(G, u) != INF){
		if(u == getSource(G)) append(L,u);
		else if (G->p[u] != NIL){
			getPath(L,G,G->p[u]);
			append(L,u);
		}
	}
	else append(L,NIL);
}
/*** Manipulation procedures ***/
void makeNull(Graph G){
	for(int i = 1; i <= getOrder(G); i++){
		clear(G->adj[i]);
		G->d[i] = INF;
		G->p[i] = NIL;
		G->color[i] = 'w';
	}
	G->source = NIL;
}
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

void BFS(Graph G, int s){
	for(int i = 1; i <= getOrder(G); i++){
		G->color[i] = 'w';
		G->d[i] = INF;
		G->p[i] = NIL;
	}
	G->source = s;
	G->color[s] = 'g';
	G->d[s] = 0;
	G->p[s] = NIL;
	List q = newList();
	append(q,s);
	int x = 0;
	int y = 0;
	List temp;
	moveFront(q);
	while(index(q) != -1){
		x = get(q);
		temp = G->adj[x];
		moveFront(temp);
		while(index(temp)!=-1){
			y = get(temp);
			if(G->color[y] == 'w'){
				G->color[y] = 'g';
				G->d[y] = G->d[x] + 1;
				G->p[y] = x;
				append(q,y);
			}
			moveNext(temp);
		}
		G->color[x] = 'b';
		moveNext(q);
	}
	freeList(&q);
}
/*** Other operations ***/
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
