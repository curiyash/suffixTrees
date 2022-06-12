#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "dnaSuffixTree.h"
#include "lca.h"
#include "dnaUtilities.h"
#include "stack.h"

int SIZE = 50000;

long long logtwo(long long a){
    int log = 0;
    while (a/2){
        a=a/2;
        log++;
    }
    return log;
}

void DisplaySparseTable(long long **st, int m, int n){
    for (int i=0; i<m; i++){
        for (int j=0; j<n; j++)
            printf("%lld ", st[i][j]);
        printf("\n");
    }
}

void EulerTour(node *n, lcaElem *lcaArr, int size, int *pos, int depth, int visited, int *visit, int *R, int *rpos){
    // DFS with labelling
    // printf("start: %d | end: %d\n", *(n->start), *(n->end));
    // printf("pos: %d\n", *pos);
    lcaArr[*pos].n = n;
    lcaArr[*pos].depth = depth;
    lcaArr[*pos].visited = visited;
    if (isLeaf(n)){
        // printf("suffixIndex: %d\n", n->suffixIndex);
        R[n->suffixIndex] = *pos;
        *rpos = *rpos+1;
    }
    // R[*rpos] = *pos;
    // printf("rpos: %d\n", rpos);
    *pos = *pos+1;
    // printf("pos: %d | depth: %d | visited: %d\n", *pos, depth, visited);
    for (int i=0; i<MAX_CHAR; i++){
        if (n->children[i]){
            printf("I'm here\n");
            EulerTour(n->children[i], lcaArr, size, pos, depth+*(n->children[i]->end)-*(n->children[i]->start)+1, ++visited, visit, R, rpos);
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

long long stMin(long long a, long long b){
    if (a<=b) return a;
    else return b;
}

long long **SparseTable(lcaElem *lcaArr, int size){
    long long **st = (long long **) malloc(sizeof(long long *)*size);
    long long K = logtwo(size)/1+1;
    for (int i=0; i<size; i++){
        st[i] = (long long *) malloc(sizeof(long long)*K);
        st[i][0] = lcaArr[i].depth;
    }
    // printf("%d\n", 1<<2);
    // printf("K: %lld\n", K);
    int m = 0;
    for (int j=1; j<K; j++){
        for (int i=0; i+(1<<j)<=size; i++){
            m = stMin(st[i][j-1], st[i+(1<<(j-1))][j-1]);
            st[i][j] = m;
            // printf("i: %d, j: %d, m: %d\n", i, j, m);
        }
    }
    return st;
}

int findMin(lcaElem *lcaArr, int *R, int u, int v, long long **st){
    int min = INT_MAX;
    int start = R[u];
    int end = R[v];
    if (start>end){
        int temp = start;
        start = end;
        end = temp;
    }

    // Sparse Table O(logn) approach
    // printf("start: %d | end: %d\n", start, end);
    int j = logtwo(end-start+1);
    // printf("j: %d | 1<<j: %d\n", j, 1<<j);
    min = stMin(st[start][j], st[end-(1<<j)+1][j]);

    // O(n) approach
    // printf("------\n");
    // printf("%d %d\n", R[u], R[v]);
    // printf("------\n");
    // lcaElem *l = (lcaElem *) malloc(sizeof(lcaElem));
    // for (int i=start; i<=end; i++){
    //     if (lcaArr[i].depth<min){
    //         l = &lcaArr[i];
    //         min = lcaArr[i].depth;
    //     }
    // }
    // printf("start: %d | end: %d\n", *(l->n->start), *(l->n->end));
    // printf("min: %d\n", min);
    return min;
}

long long **lca(suffixTree *st, long long **spt){
    lcaElem *lcaArr = (lcaElem *) malloc(sizeof(lcaElem)*SIZE);
    int *visited = (int *) calloc(SIZE, sizeof(int));
    int *R = (int *) malloc(sizeof(int)*SIZE);
    int pos = 0;
    int rpos = 0;
    printf("Here Euler\n");
    EulerTour(st->root, lcaArr, SIZE, &pos, 0, 0, visited, R, &rpos);
    DisplayLCAArr(lcaArr, pos);
    printf("\n");
    for (int i=0; i<rpos; i++){
        printf("%d ", R[i]);
    }
    printf("\n");
    // int *unique = (int *) calloc(42, sizeof(int));

    if (!spt){
        spt = SparseTable(lcaArr, pos);
    }
    // DisplaySparseTable(spt, pos, logtwo(pos));

    // For LCA, find the minimum depth between the two nodes in lcaArr
    // findMin returns lca. This will determine our lce
    // Now, for determining lps, start from 0 go till n-1, find depth of match
    // If even, we're looking at a palindrome starting i-depth+1 back of length 2*depth
    // If odd, we're looking at a palindrome centered at i-depth of length 2*depth+1
    // findMin(lcaArr, R, R, 8, 9);
    // findMin(lcaArr, R, R, 15, 16);
    // Need LCA
    // oddPal(lcaArr);
    // findMin(lcaArr, R, R, 3, 5);
    // findMin(lcaArr, R, R, 1, 3);
    // findMin(lcaArr, R, R, 4, 4);
    // findMin(lcaArr, R, R, 5, 3);

    // Example: aaab#baaa$
    // Odd matches
    // printf("Odd matches\n");
    // findMin(lcaArr, R, 0, 8, spt);
    // findMin(lcaArr, R, 1, 7, spt);
    // findMin(lcaArr, R, 2, 6, spt);
    // findMin(lcaArr, R, 3, 5, spt);
    // printf("\n");

    // printf("Even matches\n");
    // findMin(lcaArr, R, 0, 7, spt);
    // findMin(lcaArr, R, 1, 6, spt);
    // findMin(lcaArr, R, 2, 5, spt);
    // findMin(lcaArr, R, 3, 4, spt);
    // printf("\n");

    // Example: dabba#aabad$
    // printf("Odd matches\n");
    // findMin(lcaArr, R, 0, 10);
    // findMin(lcaArr, R, 1, 9);
    // findMin(lcaArr, R, 2, 8);
    // findMin(lcaArr, R, 3, 7);
    // findMin(lcaArr, R, 4, 6);
    // printf("\n");

    // printf("Even matches\n");
    // findMin(lcaArr, R, 0, 9);
    // findMin(lcaArr, R, 1, 8);
    // findMin(lcaArr, R, 2, 7);
    // findMin(lcaArr, R, 3, 6);
    // findMin(lcaArr, R, 4, 5);
    // printf("\n");

    // Example: abba#abba$
    // printf("Odd matches\n");
    // findMin(lcaArr, R, 0, 8);
    // findMin(lcaArr, R, 1, 7);
    // findMin(lcaArr, R, 2, 6);
    // findMin(lcaArr, R, 3, 5);
    // // findMin(lcaArr, R, 4, 4);
    // printf("\n");

    // printf("Even matches\n");
    // findMin(lcaArr, R, 0, 7);
    // findMin(lcaArr, R, 1, 6);
    // findMin(lcaArr, R, 2, 5);
    // findMin(lcaArr, R, 3, 4);
    // // findMin(lcaArr, R, 4, 5);
    // printf("\n");

    // Example: abba#abba$
    int len = strlen(st->str);
    int min = 0;
    stack s;
    initStack(&s);

    // min = findMin(lcaArr, R, 13, 327);
    // printf("min: %d\n", min);

    // printf("Odd matches\n");
    for (int i=0; i<(len-2)/2; i++){
        min = findMin(lcaArr, R, i, len-i-2, spt);
        // printf("%d %d %d %d | min: %d | maxLen: %d == %d?\n", i, len-i-2, R[i], R[len-i-2], min, s.maxLen, 2*min-1);
        if (2*min-1>=s.maxLen){
            if (s.maxLen==2*min-1){
                // printf("Here\n");
                push(&s, i-min+1);
            }
            else{
                // printf("maxLen: %d | min: %d\n", s.maxLen, min);
                s.maxLen = 2*min-1;
                while (!isEmpty(&s)){
                    pop(&s);
                }
                push(&s, i-min+1);
            }
        }
    }
    printf("\n");

    int flag = 0;

    // printf("Even matches\n");
    for (int i=0; i<(len-2)/2; i++){
        min = findMin(lcaArr, R, i, len-i-3, spt);
        // printf("%d %d\n", i, len-i-3);
        // printf("min: %d\n", min);
        if (2*(min-1)>=s.maxLen){
            if (s.maxLen==2*(min-1))
                push(&s, i-min+2);
            else{
                s.maxLen = 2*(min-1);
                flag = 1;
                while (!isEmpty(&s)){
                    pop(&s);
                }
                push(&s, i-min+2);
            }
        }
    }

    int c = 0;
    while (!isEmpty(&s)){
        elem *e = top(&s);
        printf("start: %d | maxLen: %d\n", e->start, s.maxLen);
        for (int i=e->start; i<e->start+s.maxLen; i++){
            printf("%c", st->str[i]);
        }
        pop(&s);
        printf("\n");
        c++;
    }
    printf("c: %d\n", c);
    printf("rpos: %d\n", rpos);
    return spt;
}

// void check(node *root, char *str){
//     for (int i=0; i<256; i++){
//         if (root->children[i]){
//             printf("parent: %d %d\n", *(root->start), *(root->end));
//             // for (int j=*(root->start); j<=*(root->end); j++){
//             //     printf("%c", str[j]);
//             // }
//             // printf("\n");
//             printf("%c\n", i);
//             printf("%d %d\n", *(root->children[i]->start), *(root->children[i]->end));
//             printf("%d\n", root->children[i]->suffixIndex);
//             check(root->children[i], str);
//         }
//     }
// }

void preprocessStringForLPS(suffixTree *st, char *str){
    int len = strlen(str)-1;
    char *temp = (char *) malloc(sizeof(char)*(len*2+3));
    for (int i=0; i<len; i++){
        temp[i] = str[i];
    }
    // printf("%s\n", temp);
    temp[len] = '#';
    for (int i=len+1; i<2*len+1; i++){
        temp[i] = str[i-2*(i-len)];
    }
    // printf("%s\n", temp);
    // printf("%d\n", 2*len+4);
    st->str = (char *) malloc(sizeof(char)*(len*2+3));
    strcpy(st->str, temp);
    // printf("hello\n");
    st->str[2*len+1] = '$';
    free(temp);
}

void DFSLPS(node *n, int *count){
    if (!n){
        return;
    }
    *count = *count+1;
    for (int i=0; i<MAX_CHAR; i++){
        DFSLPS(n->children[i], count);
    }
}

void makeComplementReverse(suffixTree *st, char *str){
    int len = strlen(str)-1;
    char *temp = (char *) malloc(sizeof(char)*(len*2+3));
    for (int i=0; i<len; i++){
        temp[i] = str[i];
    }
    // printf("%s\n", temp);
    temp[len] = '#';
    for (int i=len+1; i<2*len+1; i++){
        switch (str[i-2*(i-len)]){
            case 'A': temp[i]='T';
                    break;
            case 'G': temp[i]='C';
                    break;
            case 'C': temp[i]='G';
                    break;
            case 'T': temp[i]='A';
                    break;
            default: break;
        }
        // temp[i] = str[i-2*(i-len)];
    }
    // printf("%s\n", temp);
    // printf("%d\n", 2*len+4);
    st->str = (char *) malloc(sizeof(char)*(len*2+3));
    strcpy(st->str, temp);
    // printf("hello\n");
    st->str[2*len+1] = '$';
    free(temp);
}

// void kMisMatch(suffixTree *st, int k, int patLen){
//     lcaElem *lcaArr = (lcaElem *) malloc(sizeof(lcaElem)*SIZE);
//     int *visited = (int *) calloc(SIZE, sizeof(int));
//     int *R = (int *) malloc(sizeof(int)*SIZE);
//     int pos = 0;
//     int rpos = 0;
//     EulerTour(st->root, lcaArr, SIZE, &pos, 0, 0, visited, R, &rpos);
//     // printf("Here\n");
//     DisplayLCAArr(lcaArr, pos);
//     printf("\n");
//     for (int i=0; i<rpos; i++){
//         printf("%d ", R[i]);
//     }
//     printf("\n");

//     long long **spt = SparseTable(lcaArr, pos);
//     // DisplaySparseTable(spt, pos, logtwo(pos));

//     int p = 0;
//     int min = 0;
//     int orgk = k;
//     int i=0;
//     int strL = strlen(st->str)-patLen-1;
//     int start = 0;
//     while (i<strL){
//         if (k==0){
//             i--;
//             k = orgk;
//             start = p;
//             p = 0;
//             printf("Reset, i: %d\n", strL+p);
//         }
//         // Find LCP of i+p: and 6+p:
//         min = findMin(lcaArr, R, i, strL+p, spt);
//         printf("%d %d\n", i, strL+p);
//         printf("min: %d\n\n", min);
//         if (p+min<patLen || min==0){
//             // Partial match
//             if (!min) {
//                 k--;
//                 i++;
//                 continue;
//             }
//             p = p+min;
//             i = i+min;
//             if (!min!=0){
//                 printf("p: %d\n", p);
//             }
//             continue;
//         } else if (p+min>=patLen){
//             // printf("p: %d\n", p);
//             // Full match detected
//             printf("Found\n");
//             printf("start: %d\n", start);
//             break;
//         }
//         i++;
//     }
//     if (!k){
//         printf("Not found\n");
//     }
// }

long long **kMisMatch(suffixTree *st, int k, int patLen, long long **spt){
    lcaElem *lcaArr = (lcaElem *) malloc(sizeof(lcaElem)*SIZE);
    int *visited = (int *) calloc(SIZE, sizeof(int));
    int *R = (int *) malloc(sizeof(int)*SIZE);
    int pos = 0;
    int rpos = 0;
    EulerTour(st->root, lcaArr, SIZE, &pos, 0, 0, visited, R, &rpos);
    // printf("Here\n");
    DisplayLCAArr(lcaArr, pos);
    printf("\n");
    for (int i=0; i<rpos; i++){
        printf("%d ", R[i]);
    }
    printf("\n");

    if (!spt){
        spt = SparseTable(lcaArr, pos);
    }
    // DisplaySparseTable(spt, pos, logtwo(pos));

    int strL = strlen(st->str);
    int min = 0;
    int p = 0;
    int sec = 0;
    int mainL = strL-patLen-2;
    int foundFlag = 0;
    int prevMin = 0;
    for (int i=0; i<mainL && !foundFlag; i++){
        sec = 0;
        min = findMin(lcaArr, R, i, mainL+1, spt);
        // printf("i: %d | p: %d\n", i, mainL+1);
        // printf("min: %d\n", min);
        if (min==0){
            // printf("Not found at %d\n", i);
            continue;
        }
        if (min==patLen){
            printf("Found at %d\n", i);
            continue;
        }
        // Partial match
        p = min;
        sec = i+p;
        prevMin = min;
        // printf("p: %d | sec: %d\n", min, sec);
        for (int j=0; j<k+1; j++){
            min = findMin(lcaArr, R, sec, mainL+p+1, spt);
            // printf("i: %d | p: %d\n", sec, mainL+p+1);
            // printf("min: %d\n", min);
            p+=min;
            if (p==patLen){
                printf("Found at %d\n", i);
                // p = prevMin;
                // foundFlag = 1;
                break;
            }
            sec+=p;
        }
        // printf("Not found at %d\n", i);
    }
    // printf("Not found\n");
    return spt;
}

void preprocessStringForKMisMatch(suffixTree *st, char *str, char *pat){
    int len = strlen(str)-1;
    int patLen = strlen(pat);
    char *temp = (char *) malloc(sizeof(char)*(len+patLen+3));
    for (int i=0; i<len; i++){
        temp[i] = str[i];
    }
    // printf("%s\n", temp);
    temp[len] = '#';
    for (int i=len+1; i<len+patLen+1; i++){
        temp[i] = pat[i-len-1];
    }
    temp[len+patLen+1] = '$';
    st->str = (char *) malloc(sizeof(char)*(len*2+3));
    strcpy(st->str, temp);
    free(temp);
}

// int main(){
//     suffixTree st;
//     initSuffixTree(&st, 7, 1);
//     printf("MAX_CHAR: %d\n", MAX_CHAR);
//     // Test case
//     // char *str = "AATGTACGATCGTACTAT";
//     char *str = "AGATTAGC#AAC";
//     // preprocessStringForLPS(&st, str);
//     preprocessString(&st, str);
//     // makeComplementReverse(&st, str);
//     // free(str);
//     printf("%s\n", st.str);
//     buildSuffixTree(&st);

//     // check(st.root, st.str);
//     // printf("-----------------------------\n");
//     // lca(&st);
//     // int count = 0;
//     // DFSLPS(st.root, &count);
//     // printf("count: %d\n", count);
//     // longestCommonSubstring(&st);
//     kMisMatch(&st, 3, 3);
//     // printf("%lld\n", logtwo(42));
//     return 0;
// }
