/* 
 * File:   Graph.c
 * Author: Pit Apps
 */

#include "List.h"
#include "Graph.h"

typedef struct Graph {
    ListRef *adj;
    int *colour;
    int *parent;
    int *discover;
    int *finish;
    int order;
    int size;
} Graph;

enum colours {
    WHITE, GRAY, BLACK
};

/*** Constructors-Destructors ***/

/* Returns GraphRef representing graph having n vertices and 
 * no edges */
GraphRef newGraph(int n) {
    GraphRef G = malloc(sizeof (Graph));
    G->colour = calloc(n + 1, sizeof (int));
    G->discover = calloc(n + 1, sizeof (int));
    G->finish = calloc(n + 1, sizeof (int));
    G->adj = calloc(n + 1, sizeof (ListRef));
    G->parent = calloc(n + 1, sizeof (int));
    G->order = n;
    G->size = 0;

    int i;
    for (i = 1; i <= n; i++) {
        G->adj[i] = newList();
        G->parent[i] = NIL;
        G->discover[i] = UNDEF;
        G->finish[i] = UNDEF;
    }

    return G;
}

/* Frees all dynamic memory associated with *pG; sets *pG to NULL */
void freeGraph(GraphRef* pG) {
    if (pG != NULL && *pG != NULL) {
        free((*pG)->discover);
        free((*pG)->finish);
        free((*pG)->colour);
        free((*pG)->parent);

        int i;
        for (i = 0; i <= (*pG)->order; i++) {
            freeList(&((*pG)->adj[i]));
        }
        free((*pG)->adj);

        free(*pG);
        *pG = NULL;
    }
}

/*** Access functions ***/

/* Returns order of GraphRef G */
int getOrder(GraphRef G) {
    if (G == NULL) {
        printf("Graph Error: calling getOrder() on NULL GraphRef\n");
        exit(1);
    }
    return G->order;
}

/* Returns size of GraphRef G */
int getSize(GraphRef G) {
    if (G == NULL) {
        printf("Graph Error: calling getSize() on NULL GraphRef\n");
        exit(1);
    }
    return G->size;
}

/* Returns parent of vertex u
 * Pre: 1<=u<=getOrder(G) */
int getParent(GraphRef G, int u) {
    if (G == NULL) {
        printf("Graph Error: calling getParent() on NULL GraphRef\n");
        exit(1);
    }

    if (!(1 <= u && u <= getOrder(G))) {
        printf("Graph Error: calling getParent() on vertex that is outside the Graph!\n");
        exit(1);
    }

    return G->parent[u];
}

/* Returns discover time of vertex u
 * Pre: 1<=u<=getOrder(G) */
int getDiscover(GraphRef G, int u) {
    if (G == NULL) {
        printf("Graph Error: calling getDiscover() on NULL GraphRef\n");
        exit(1);
    }

    if (!(1 <= u && u <= getOrder(G))) {
        printf("Graph Error: calling getDiscover() on vertex that is outside the Graph!\n");
        exit(1);
    }
    return G->discover[u];
}

/* Returns finish time of vertex u
 * Pre: 1<=u<=getOrder(G) */
int getFinish(GraphRef G, int u) {
    if (G == NULL) {
        printf("Graph Error: calling getFinish() on NULL GraphRef\n");
        exit(1);
    }

    if (!(1 <= u && u <= getOrder(G))) {
        printf("Graph Error: calling getFinish() on vertex that is outside the Graph!\n");
        exit(1);
    }

    return G->finish[u];
}

/*** Manipulation procedures ***/

/* Inserts new directed edge from u to v */
void addArc(GraphRef G, int u, int v) {
    if (G == NULL) {
        printf("Graph Error: calling addArc() on NULL GraphRef\n");
        exit(1);
    }

    insertBack(G->adj[u], v);
    G->size++;
}

/* Recursive function which is part of DFS, takes in the current time variable and stack and visits x */
void Visit(GraphRef G, ListRef S, int x, int *time) {
    G->colour[x] = GRAY;
    G->discover[x] = ++(*time);

    int y;
    ListRef L = G->adj[x];
    moveTo(L, 0);
    while (!offEnd(L)) {
        y = getCurrent(L);
        if (G->colour[y] == WHITE) {
            G->parent[y] = x;
            Visit(G, S, y, time);
        }
        moveNext(L);
    }
    G->colour[x] = BLACK;
    G->finish[x] = ++(*time);
    insertAfterCurrent(S, x); /* insert item to the front of the *new* stack */
}

/* Runs BFS algorithm 
 * Pre: getLength(S)==getOrder(G) */
void DFS(GraphRef G, ListRef S) {
    if (G == NULL) {
        printf("Graph Error: calling DFS() on NULL GraphRef\n");
        exit(1);
    }

    if (S == NULL) {
        printf("Graph Error: calling DFS() on NULL ListRef\n");
        exit(1);
    }

    if (getLength(S) != getOrder(G)) {
        printf("Graph Error: calling DFS() on on unequal Graph/List size.\n");
        exit(1);
    }

    int i, last;
    int time = 0;

    /* Initialise */
    for (i = 1; i <= G->order; i++) {
        G->colour[i] = WHITE;
        G->parent[i] = NIL;
    }

    /* Main Loop */
    last = getBack(S);
    moveTo(S, getLength(S) - 1); /* move pointer to the back of the stack */
    do {
        i = getFront(S);
        if (G->colour[i] == WHITE) {
            Visit(G, S, i, &time);
        }
        deleteFront(S);
    } while (i != last);
}

/*** Other operations ***/

/* Returns handle to a new graph representing the transpose of G */
GraphRef transpose(GraphRef G) {
    if (G == NULL) {
        printf("Graph Error: calling transpose() on NULL GraphRef\n");
        exit(1);
    }

    GraphRef nG = newGraph(G->order);

    ListRef L;
    int i;

    for (i = 1; i <= G->order; i++) {

        L = G->adj[i];
        moveTo(L, 0);
        while (!offEnd(L)) {
            addArc(nG, getCurrent(L), i);
            moveNext(L);
        }
    }

    return nG;
}

/* Returns a copy of G */
GraphRef copyGraph(GraphRef G) {
    if (G == NULL) {
        printf("Graph Error: calling copyGraph() on NULL GraphRef\n");
        exit(1);
    }

    GraphRef nG = newGraph(G->order);
    ListRef L;
    int i;

    for (i = 1; i <= G->order; i++) {
        nG->colour[i] = G->colour[i];
        nG->discover[i] = G->discover[i];
        nG->finish[i] = G->finish[i];
        nG->parent[i] = G->parent[i];

        L = G->adj[i];
        moveTo(L, 0);
        while (!offEnd(L)) {
            addArc(nG, i, getCurrent(L));
            moveNext(L);
        }
    }

    return nG;
}

/* Prints adjacency list of G to out */
void printGraph(FILE* out, GraphRef G) {
    if (G == NULL) {
        printf("Graph Error: calling printGraph() on NULL GraphRef\n");
        exit(1);
    }

    if (out == NULL) {
        printf("Graph Error: calling printGraph() on NULL FILE pointer\n");
        exit(1);
    }

    int i, j;
    char str[1 + (2 * G->order)];
    ListRef L;
    for (i = 1; i <= G->order; i++) {
        *str = 0;
        L = G->adj[i];
        sprintf(str, "%d:", i);
        for (j = 0; j < getLength(L); j++) {
            moveTo(L, j);
            sprintf(str, "%s %i", str, getCurrent(L));
        }
        fprintf(out, "%s\n", str);
    }
}
