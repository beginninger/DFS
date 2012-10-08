/* 
 * File:   Graph.h
 * Author: Pit Apps
 */

#ifndef GRAPH_H
#define	GRAPH_H
#define UNDEF -1
#define NIL -2

typedef struct Graph* GraphRef;

/*** Constructors-Destructors ***/
GraphRef newGraph(int n);
void freeGraph(GraphRef* pG);

/*** Access functions ***/
int getOrder(GraphRef G);
int getSize(GraphRef G);
int getParent(GraphRef G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getDiscover(GraphRef G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getFinish(GraphRef G, int u); /* Pre: 1<=u<=n=getOrder(G) */

/*** Manipulation procedures ***/
void addArc(GraphRef G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void DFS(GraphRef G, ListRef S); /* Pre: getLength(S)==getOrder(G) */

/*** Other operations ***/
GraphRef transpose(GraphRef G);
GraphRef copyGraph(GraphRef G);
void printGraph(FILE* out, GraphRef G);

#endif	/* GRAPH_H */
