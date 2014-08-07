/* 
 * File:   GraphTest.c
 * Author: Pit Apps
 */

#include "List.h"
#include "Graph.h"

int main(int argc, char* argv[]) {
    GraphRef G = newGraph(11);
    ListRef L = newList();

    printf("Adding edges\n");
    addArc(G, 1, 2);
    addArc(G, 2, 5);
    addArc(G, 3, 2);
    addArc(G, 3, 5);
    addArc(G, 4, 1);
    addArc(G, 5, 4);
    addArc(G, 6, 3);
    addArc(G, 6, 5);
    addArc(G, 6, 9);
    addArc(G, 6, 10);
    addArc(G, 7, 3);
    addArc(G, 7, 6);
    addArc(G, 8, 4);
    addArc(G, 9, 4);
    addArc(G, 9, 5);
    addArc(G, 9, 8);
    addArc(G, 10, 9);
    addArc(G, 10, 11);
    addArc(G, 11, 7);

    int s;
    for (s = 1; s <= 11; s++) {
        insertBack(L, s);
    }

    printf("Printing Graph\n");
    printGraph(stdout, G);

    printf("Running DFS\n");
    DFS(G, L);

    printf("Printing List\n");
    printList(stdout, L);
    printf("\n");

    printf("Transpose\n");
    GraphRef gT = transpose(G);

    printf("Printing Transposed Graph\n");
    printGraph(stdout, gT);

    printf("DFS on Transposed Graph\n");
    DFS(gT, L);

    printf("Printing List\n");
    printList(stdout, L);
    printf("\n");

    printf("Order of G: %d, size: %d.\n", getOrder(G), getSize(G));
    printf("Order of gT: %d, size: %d.\n", getOrder(gT), getSize(gT));

    printf("Parent of 5: %d, discover of 5: %d, finish of 5: %d.\n", getParent(G, 5), getDiscover(G, 5), getFinish(G, 5));
    
    printf("Copying G:\n");
    GraphRef gC = copyGraph(G);
    printGraph(stdout, gC);

    freeList(&L);
    freeGraph(&gT);
    freeGraph(&gC);
    freeGraph(&G);
    return 0;
}
