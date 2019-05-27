#pragma once


typedef struct List List;
struct List {
    int from;
    int to;
    int length;
};

typedef struct Graph Graph;
struct Graph {
    int V;
    int E;
    List *Edge;
};

typedef struct Disjoint_Sets D_Sets;
struct Disjoint_Sets {
    int val;
    int rank;
    D_Sets *parent;
};


void kruskal(char *in, char *out);

Graph *create_graph(unsigned int n_vertices, unsigned int n_edges);

D_Sets *fix_parent(D_Sets *set);

void union_sets(D_Sets *sets, int x, int y);

void psort(Graph *G);

void sift(Graph *G, unsigned int i, unsigned int n);

void errors(int err);

void free_graph(Graph *G);

void print_help(void);