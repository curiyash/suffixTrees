#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "suffixTree.h"
#include "lca.h"
#include "utilities.h"

void EulerTour(node *n, lcaElem *lcaArr, int size, int *pos, int depth, int visited, int *visit, int *R, int *rpos){
    // DFS with labelling
    // printf("start: %d | end: %d\n", *(n->start), *(n->end));
    // printf("pos: %d\n", *pos);
    lcaArr[*pos].n = n;
    lcaArr[*pos].depth = depth;
    lcaArr[*pos].visited = visited;
    R[*rpos] = *pos;
    // printf("rpos: %d\n", rpos);
    *rpos = *rpos+1;
    *pos = *pos+1;
    // printf("pos: %d | depth: %d | visited: %d\n", *pos, depth, visited);
    for (int i=0; i<MAX_CHAR; i++){
        if (n->children[i]){
            EulerTour(n->children[i], lcaArr, size, pos, depth+1, ++visited, visit, R, rpos);
            lcaArr[*pos].n = n;
            lcaArr[*pos].depth = depth;
            lcaArr[*pos].visited = visited;
            *pos = *pos+1;
        }
    }
}

void DisplayLCAArr(lcaElem *arr, int size){
    for (int i=0; i<size; i++){
        // printf("---\n");
        // printf("%d %d\n", *(arr[i].n->start), *(arr[i].n->end));
        printf("%d ", arr[i].depth);
        // printf("%d\n", arr[i].visited);
        // printf("---\n");
    }
    printf("\n");
}

void findMin(lcaElem *lcaArr, int *R, int u, int v){
    int min = INT_MAX;
    int start = R[u];
    int end = R[v];
    lcaElem *l = (lcaElem *) malloc(sizeof(lcaElem));
    for (int i=start; i<=end; i++){
        if (lcaArr[i].depth<min){
            l = &lcaArr[i];
            min = lcaArr[i].depth;
        }
    }
    printf("start: %d | end: %d\n", *(l->n->start), *(l->n->end));
    printf("min: %d\n", min);
}

void lca(suffixTree *st){
    lcaElem *lcaArr = (lcaElem *) malloc(sizeof(lcaElem)*42);
    int *visited = (int *) calloc(42, sizeof(int));
    int *R = (int *) malloc(sizeof(int)*42);
    int pos = 0;
    int rpos = 0;
    EulerTour(st->root, lcaArr, 42, &pos, 0, 0, visited, R, &rpos);
    // printf("Here\n");
    DisplayLCAArr(lcaArr, pos);
    for (int i=0; i<rpos; i++){
        printf("%d ", R[i]);
    }
    printf("\n");
    // int *unique = (int *) calloc(42, sizeof(int));

    // For LCA, find the minimum depth between the two nodes in lcaArr
    findMin(lcaArr, R, 8, 9);
    findMin(lcaArr, R, 15, 16);
    // findMin(lcaArr, R, 3, 5);
    // findMin(lcaArr, R, 1, 3);
    // findMin(lcaArr, R, 4, 4);
    // findMin(lcaArr, R, 5, 3);
}

void check(node *root, char *str){
    for (int i=0; i<256; i++){
        if (root->children[i]){
            printf("parent: %d %d\n", *(root->start), *(root->end));
            // for (int j=*(root->start); j<=*(root->end); j++){
            //     printf("%c", str[j]);
            // }
            // printf("\n");
            printf("%c\n", i);
            printf("%d %d\n", *(root->children[i]->start), *(root->children[i]->end));
            check(root->children[i], str);
        }
    }
}

int main(){
    suffixTree st;
    initSuffixTree(&st);
    char *str = "aaaabbaa#aabbaaaa";
    preprocessString(&st, str);
    printf("%s\n", st.str);
    buildSuffixTree(&st);

    // check(st.root, st.str);
    // printf("-----------------------------\n");
    // lca(&st);
    longestCommonSubstring(&st);
    return 0;
}
