//
// Created by Ba Tuan on 26/11/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 1000/* nombre maximum de sommets */
typedef struct {
    int A[NMAX][NMAX]; /* matrice carree 0/1 */;
    int n; /* valeur comprise entre 0 et NMAX */
    int edges;
    int comp[NMAX][NMAX];
    int compEdges;
} GRAPH;

typedef struct {
    int clauses;
    int variables;
    int * value[];
} SAT;

/*
 * Encoding MaxIS. By noting the relationship between vertex covers and independent sets, we can adapt the previous
 * encoding. For any (u, v) ∈ E, add a hard clause (¬xu ∨ ¬xv).
 * For each u ∈ V , add a soft clause (xu).
 */
GRAPH graph;

//complement
void graph2CNF() {
    char * fileName = "graph2CNFcom1.txt";
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        fprintf(stderr,"Error in creating %s: aborting.\n", fileName);
        exit(-1);
    }
    char mm[255];
    sprintf(mm, "p cnf %d %d\n", graph.n, graph.compEdges + 1);
    fprintf(file,"%s", mm);
    for (int i = 0; i < graph.n; ++i) {
        sprintf(mm, "%d ", i+1);
        fprintf(file, "%s", mm);
    }
    fprintf(file, "%d\n", 0);

    // For any (u, v) ∈ E, add a hard clause (¬xu ∨ ¬xv).
    for (int i = 0; i < graph.n; i++) {
        for (int j = i; j < graph.n; j++) {
            if (graph.comp[i][j] == 1) {
                sprintf(mm, "%d %d 0\n", -(i+1), -(j+1));
                fprintf(file, "%s", mm);
            }
        }
    }
    fclose(file);
}


void graph2CNFAHunt(int k, char* fileName) {
    char path[100] = "./test/graph2cnf/";
    strcat(path, fileName);
    strcat(path, "2CNF.txt");
    printf("path file: %s", path);
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr,"Error in creating %s: aborting.\n", fileName);
        exit(-1);
    }
    char mm[255];
    char *preamble = NULL;
    int count = 0;
    // CONSTRAINT 1: ∀{i |1 ≤ i ≤ k}∃xiv
    /**
     * # CONSTRAINT 1)
		# We want to say, for every i, there is an ith vertex in the clique
		# equivalent to: v0 is 1st or v1 is 1st or... vn is 1st
		# AND
		# v0 is 2nd or v1 is 2nd or v2 is 2nd.... etc
		# -> encode
		#  (x_1,0) or (x_1,1) or (x_1,2).... (x_1, n) where n = last vertex
		# AND
		# (x_2,0) or (x_2,1) or (x_2,2).... (x_2, n) where n = last vertex
		# AND
		# ...
		# (x_k,0) or (x_k,1) or (x_k,2).... (x_k, n) where n = last vertex

		# can create literals and encode constraint 1 using the same loop
		# since they require the same algorithm structure
     */
    preamble = (char *)calloc(10000, sizeof(char));
    for(int i = 0; i < k; i++) {
        for(int j = 0; j<graph.n; j++) {
            sprintf(mm, "%d ", i*graph.n + j + 1);
            strcat(preamble, mm);
        }
        strcat(preamble, "0\n");
        count ++;
    }

    /**
     * Encode Constraint 2
     * For every non-edge (v, w) ∈ E, v AND w cannot BOTH be in the clique (since there
     * must exist an edge between any pair of vertices in (v, w) ∈ C). We can write this more
     * formally as
     * ∀{i, j | i != j} ∀{v, w ∈ V |(v, w) ∈/ E ∧ v != w} ¬xiv ∨ ¬xjw
     */

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            for (int v = 0; v < graph.n; v++) {
                for (int w = 0; w < graph.n; w++) {
                    if(i!=j&&v!=w) {
                        if(graph.A[v][w]==0) {
                            sprintf(mm, "%d %d 0\n", -(i*graph.n + v + 1), -(j*graph.n + w + 1));
                            strcat(preamble,mm);
                            count ++;
                        }
                    }
                }
            }
        }
    }

    /**
     * Encode Constraint 3
     * For every i, j (where j 6= i), the i th vertex is different from the j th vertex.
     * That is, we know that a vertex v cannot be both the i th and the j th vertex in the clique.
     * It also means that two different vertices cannot both be the i th vertex in the clique.
     * We can write this as a two-part constraint for simplicity:
     * ∀[i, j | i != j] ∀[v ∈ V ] ¬xiv ∨ ¬xjv
     * ∀i ∀[v, w ∈ V | v != w] ¬xiv ∨ ¬xiw
     */

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            for (int v = 0; v < graph.n; v++) {
                if(i!=j) {
                    sprintf(mm, "%d %d 0\n", -(i*graph.n + v + 1), -(j*graph.n + v + 1));
                    strcat(preamble,mm);
                    count ++;
                }
            }
        }
    }
    //∀i ∀[v, w ∈ V | v != w] ¬xiv ∨ ¬xiw
    for (int v = 0; v < graph.n; v++) {
        for (int w = 0; w < graph.n; w++) {
            for (int i = 0; i < k; i++) {
                if(v!=w) {
                    sprintf(mm, "%d %d 0\n", -(i*graph.n + v + 1), -(i*graph.n + w + 1));
                    strcat(preamble,mm);
                    count ++;
                }
            }
        }
    }
    // Print head of file
    sprintf(mm, "p cnf %d %d\n", graph.n * k, count);
    printf("\np cnf %d %d\n", graph.n * k, count);
    fprintf(file,"%s", mm);
    fprintf(file, "%s", preamble);
    fclose(file);
}

void readGraph(char* path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr,"Error in creating %s: aborting.\n", path);
        exit(-1);
    }
    int i, j;
    int edges=0, n=0;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {
        if(sscanf(line, "p edge %d %d", &n, &edges) == 2) {
            printf("edges: %d, n: %d \n", edges, n);
            break;
        }
    }
    graph.edges = edges;
    graph.n = n;
    int count = 0;
    for (int k = 0; k < edges; ++k) {
        int d = fscanf(file, "e %d %d\n", &i, &j);
        if (d == 2) {
            graph.A[i-1][j-1] = 1;
            graph.A[j-1][i-1] = 1;
            count++;
        }
    }
    if(count == edges) {
        printf("Get Graph Complete, generate complement.\n");
        int compEdges = 0;
        for(int i = 0; i < graph.n; i ++) {
            for (int j = 0; j < graph.n; j++) {
                if( i == j) graph.comp[i][j] = 0;
                else {
                    graph.comp[i][j] = 1 - graph.A[i][j];
                    if(graph.comp[i][j]==1) compEdges++;
                }
            }
        }
        graph.compEdges = compEdges/2;
    }
    fclose(file);
}

int main(int argc, char *argv[]){
    if( argc != 3 ) {
        printf("error: for using, type: \n./main graph.txt k\n");
        return 0;
    }
    char * fileName = argv[1];
    int k = strtol(argv[2], NULL, 10);
    char path[100] = "./test/datagraph/";
    strcat(path, fileName);
    printf("path: %s, k = %d\n", path, k);
    readGraph(path);
    graph2CNFAHunt(k, fileName);
    return 0;
}