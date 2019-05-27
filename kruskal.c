#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


void sift(Graph *G, unsigned int i, unsigned int n) {
    unsigned int l,
            r,
            k;
    List tmp;
    i++;
    while ((l = 2 * i) <= n) {
        r = (l + 1 <= n) ? l + 1 : i;
        if ((G->Edge[i - 1].length >= G->Edge[l - 1].length) && (G->Edge[i - 1].length >= G->Edge[r - 1].length))
            return;
        k = (G->Edge[l - 1].length >= G->Edge[r - 1].length) ? l : r;
        tmp = G->Edge[i - 1];
        G->Edge[i - 1] = G->Edge[k - 1];
        G->Edge[k - 1] = tmp;
        i = k;
    }
}

void psort(Graph *G) {
    int i;
    List tmp;

    for (i = G->E / 2; i >= 0; i--) {
        sift(G, i, G->E);
    }
    for (i = G->E - 1; i > 0; i--) {

        tmp = G->Edge[0];
        G->Edge[0] = G->Edge[i];
        G->Edge[i] = tmp;
        sift(G, 0, i);
    }
}

void union_sets(D_Sets *Set, int x, int y) {
    if (Set[x].rank > Set[y].rank) {
        Set[y].parent = Set + x;
    } else {
        Set[x].parent = Set + y;
        Set[y].rank = (Set[y].rank == Set[x].rank) ? Set[y].rank + 1 : Set[y].rank;
    }
}

D_Sets *fix_parent(D_Sets *set) {
    D_Sets *p = set->parent;
    if (p == set) {
        return p;
    }
    set->parent = fix_parent(set->parent);
    return set->parent;
}

void kruskal(char *in, char *out) {
    int n_vertices,
            n_edges,
            i;
    unsigned int err;

    FILE *f1 = fopen(in, "rb");
    FILE *f2 = fopen(out, "wb");
    if (f1 == NULL) {
        errors(1);
        return;
    }
    if (f2 == NULL) {
        errors(1);
        return;
    }
    fseek(f1, 0, SEEK_END);
    int size = ftell(f1);
    if (size == 0) {
        errors(2);
        return;
    }
    fseek(f1, 0, SEEK_SET);

    if (fscanf(f1, "%d", &n_vertices)) {
        if (n_vertices < 0 || n_vertices > 5000) {
            errors(2);
            return;
        }
    } else {
        errors(6);
        return;
    }

    if (fscanf(f1, "%d", &n_edges) != 1) {
        errors(6);
        return;
    }
    if (n_edges < 0 || n_edges > n_vertices * (n_vertices - 1) / 2) {
        errors(3);
        return;
    }

    Graph *G = create_graph(n_vertices, n_edges);

    int v1, v2, length;

    for (i = 0; i < G->E; i++) {
        if (fscanf(f1, "%d %d %d", &v1, &v2, &length) == 3) {
            if (v1 < 1 || v1 > G->V ||
                v2 < 1 || v2 > G->V) {
                free_graph(G);
                errors(4);
                return;
            }
        } else {
            free_graph(G);
            errors(6);
            return;
        }
        if (length < 0 || length > INT_MAX) {
            errors(5);
            return;
        }
        G->Edge[i].from = v1;
        G->Edge[i].to = v2;
        G->Edge[i].length = length;


    }

    psort(G);

    D_Sets *Set = (D_Sets *) malloc((G->V + 1) * sizeof(D_Sets));

    for (i = 1; i <= G->V; i++) {
        Set[i].val = i;
        Set[i].rank = 0;
        Set[i].parent = Set + i;
    }

    int *index = (int *) malloc((G->V + 1) * sizeof(int));

    i = 0;
    unsigned int j = 1;
    int set1, set2;
    D_Sets *p1, *p2;

    for (i = 0; i < G->E; i++) {
        p1 = &Set[G->Edge[i].from];
        p2 = &Set[G->Edge[i].to];
        p1->parent = fix_parent(p1);
        p2->parent = fix_parent(p2);

        set1 = p1->parent->val;
        set2 = p2->parent->val;
        if (set1 != set2) {
            union_sets(Set, set1, set2);
            index[j] = i;
            j++;
        }
    }

    for (i = 1; i < G->V; i++)
        fix_parent(&Set[i]);

    int k = 1;
    for (i = 1; i < G->V; i++) {
        if (Set[i].parent->val == Set[i + 1].parent->val)
            k++;
    }
    if ((G->V == 0) || (G->V > 1 && G->E == 0) || k != G->V) {
        errors(7);
        free_graph(G);
        free(Set);
        free(index);
        return;
    }

    for (i = 1; i < G->V; i++)
        fprintf(f2, "%d %d\n", G->Edge[index[i]].from, G->Edge[index[i]].to);

    free_graph(G);
    free(index);
    free(Set);
    fclose(f1);
    fclose(f2);

}

Graph *create_graph(unsigned int n_vertices, unsigned int n_edges) {
    unsigned int i;
    Graph *G = (Graph *) malloc(sizeof(Graph));
    G->V = n_vertices;
    G->E = n_edges;

    G->Edge = (List *) malloc((G->E + 1) * sizeof(List));

    return G;
}

void free_graph(Graph *G) {
    unsigned int i;
    free(G->Edge);
    free(G);
}


void errors(int err) {
    switch (err) {
        case 1:
            printf("file cannot be opened");
            break;
        case 2:
            printf("bad number of vertices");
            break;
        case 3:
            printf("bad number of edges");
            break;
        case 4:
            printf("bad vertex");
            break;
        case 5:
            printf("bad length");
            break;
        case 6:
            printf("bad number of lines");
            break;
        case 7:
            printf("no spanning tree");
            break;
        default:;
    }
}

void print_help(void) {
    printf("Kruskal: <inputfile> <outputfile>");
}
