//----------------------------------------------
// Graph.h
// Dmitriy Nikitin ID:1649206 PA5
//----------------------------------------------
#ifndef _Graph_H_INCLUDE_
#define _GRAPH_H_INCLUDE_
#include <stdio.h>
#include "List.h"
#define UNDEF -1
#define NIL -0

typedef struct GraphObj* Graph;
/*** Constructors-Destructors ***/
Graph newGraph(int n);
void freeGraph(Graph* pG);

/*** Access functions ***/
int getOrder(Graph G);
int getSize(Graph G);
int getParent(Graph G, int u);
int getDiscover(Graph G, int u);
int getFinish(Graph G, int u);

/*** Manipulation procedures ***/
void addArc(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void addEdge(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void DFS(Graph G, List S); /* Pre: length(S)==getOrder(G) */

/*** Other operations ***/
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE* out, Graph G);
#endif