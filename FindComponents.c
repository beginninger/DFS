/* 
 * File:   FindComponents.c
 * Author: Pit Apps
 */

#include "List.h"
#include "Graph.h"
#define MAX_LEN 200 /* should be enough for most input files */

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Usage: %s infile outfile\n", argv[0]);
        exit(1);
    }

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    if (in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }
    if (out == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }

    char line[MAX_LEN];
    fgets(line, MAX_LEN, in); /* number of vertices */
    int n = atoi(line);
    GraphRef G = newGraph(n);
    ListRef S = newList();
    int s, d;

    /* Filling the graph */
    while (fgets(line, MAX_LEN, in) != NULL) {

        s = atoi(strtok(line, " "));
        d = atoi(strtok(NULL, " "));
        if (s == 0 && d == 0) {
            break;
        }

        addArc(G, s, d);
    }

    /* Filling the Stack from 1...n */
    for (s = 1; s <= n; s++) {
        insertBack(S, s);
    }

    /* Print adjacency list */
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);

    /* Execute DFS first time */
    DFS(G, S);

    /* Create transpose of G */
    GraphRef tG = transpose(G);

    /* Execute DFS second time */
    DFS(tG, S);

    /* Find and put results into temp strings */
    /* This would be much simpler if it didn't have to resemble the example txt */
    moveTo(S, getLength(S) - 1);
    char str[12 + getLength(S)*2];
    char result[15 * getLength(S)];
    char intB[12 + getLength(S)*2];
    *str = 0;
    *intB = 0;
    *result = 0;
    s = 0;
    while (!offEnd(S)) {
        d = getCurrent(S);
        if (getParent(tG, d) == NIL) {
            s++;
            sprintf(result, "%sComponent %d: %d%s\n", result, s, d, str);
            *str = 0;
        } else {
            /* Stupid hack so that formatting is the same as in example */
            sprintf(intB, " %d", d);
            strcat(intB, str);
            strcpy(str, intB);
        }
        movePrev(S);
    }

    /* Write results */
    fprintf(out, "\nG contains %d strongly connected components:\n%s", s, result);

    fclose(in);
    fclose(out);
    freeGraph(&tG);
    freeGraph(&G);
    freeList(&S);

    return 0;
}
