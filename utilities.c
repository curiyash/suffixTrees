#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "suffixTree.h"
#include "utilities.h"
#include "queue.h"

int match(char *pat, int posStart, int start, int end, char *str, int *index){
    // 1 = full match
    // 0 = partial match
    // -1 = no match
    int pos = posStart;
    int match = 0;
    for (int j=start; j<=end && pos<strlen(pat); j++){
        printf("Is %c==%c?\n", pat[pos], str[j]);
        if (pat[pos]!=str[j]){
            return -1;
        } else{
            match++;
            *(index) = *(index)+1;
            pos++;
        }
    }
    printf("matches: %d, len: %ld\n", match, strlen(pat));
    if (match!=strlen(pat)-posStart){
        return 0;
    }
    return 1;
}

int checkForSubString(suffixTree st, char *pat){
    node *curr = st.root;
    int index = 0;
    int update = 0;
    int len = strlen(pat);
    int i = 0;
    int finalStatus = -1;
    int partial = 0;
    while (i<MAX_CHAR && index<len){
        if (i==0){
            printf("Yes\n");
            printf("%d %d %d\n", *(curr->start), *(curr->end), index);
        }
        if (curr->children[i]){
            // If pat doesn't match child label
            printf("Checking: %d %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
            int status = match(pat, index, *(curr->children[i]->start), *(curr->children[i]->end), st.str, &update);
            printf("status: %d %d\n", status, update);
            finalStatus = status;
            if (status==-1) {
                i++;
                continue;
            };
            // If full match
            if (status==1){
                printf("Is a substring\n");
                return 1;
            }
            // If partial match
            if (status==0){
                curr = curr->children[i];
                i = 0;
                index = update;
                partial = 1;
                continue;
            }
        }
        i++;
    }
    if (finalStatus==1){
        printf("Is a substring\n");
        return 1;
    } else if (partial){
        printf("Detected a partial match\n");
        return 0;
    }else{
        printf("Not a substring\n");
        return 0;
    }
}

int findLeaves(node *n){
    int leaves = 0;
    // Is the suffix index not -1
        // Then leaf
    if (n->suffixIndex!=-1){
        return 1;
    }
    for (int i=0; i<MAX_CHAR; i++){
        if (n->children[i]){
            leaves += findLeaves(n->children[i]);
        }
    }
    return leaves;
}

int countOccurences(suffixTree *st, char *pat){
    node *curr = st->root;
    int index = 0;
    int update = 0;
    int len = strlen(pat);
    int i = 0;
    int finalStatus = -1;
    int count = 0;
    while (i<MAX_CHAR && index<len){
        if (i==0){
            printf("Yes\n");
            printf("%d %d %d\n", *(curr->start), *(curr->end), index);
        }
        if (curr->children[i]){
            // If pat doesn't match child label
            printf("Checking: %d %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
            int status = match(pat, index, *(curr->children[i]->start), *(curr->children[i]->end), st->str, &update);
            printf("status: %d %d\n", status, update);
            finalStatus = status;
            if (status==-1) {
                i++;
                continue;
            };
            // If full match
            if (status==1){
                count = findLeaves(curr->children[i]);
                return count;
            }
            // If partial match
            if (status==0){
                curr = curr->children[i];
                i = 0;
                index = update;
                continue;
            }
        }
        i++;
    }
    if (finalStatus==1){
        printf("Is a substring\n");
        return count;
    } else{
        printf("Not a substring\n");
        return 0;
    }
}

// int isLeaf(node *n){
//     if (n->suffixIndex!=-1){
//         return 1;
//     } else{
//         return 0;
//     }
// }

int pathLength(node *n){
    return *(n->end)-*(n->start)+1;
}

int isRoot(node *n){
    if (n){
        if (*(n->end)==-1){
            return 1;
        } else{
            return 0;
        }
    }  else{
        return 0;
    }
}

void DFS(node *n, int *len, int currPathLen){
    if (isLeaf(n)){
        return;
    }
    // printf("currentPathLen: %d, pathLen: %d\n", currPathLen, *len);
    printf("start: %d, end: %d\n", *(n->start), *(n->end));
    if (!isRoot(n)){
        if (currPathLen + pathLength(n)>*len){
            *len = currPathLen + pathLength(n);
        }
        currPathLen = pathLength(n);
    }
    printf("currentPathLen: %d, pathLen: %d\n", currPathLen, pathLength(n));
    for (int i=0; i<MAX_CHAR; i++){
        if (!isLeaf(n->children[i])){
            DFS(n->children[i], len, currPathLen);
        }
    }
}

int longestRepeatedSubstring(suffixTree *st){
    int len = 0;
    int currPathLen = 0;
    DFS(st->root, &len, currPathLen);
    return len;
}

int process(node *n, char *str){
    int candidate = 0;
    int candidacy[2] = {0,0};
    for (int i=0; i<MAX_CHAR; i++){
        if (n->children[i]){
            if (isLeaf(n->children[i])){
                printf("Leaf\n");
                if (str[*(n->children[i]->end)]=='$'){
                    printf("returned $\n");
                    candidacy[0] = 1;
                } else{
                    printf("returned #\n");
                    candidacy[1] = 1;
                }
            } else {
                printf("Internal node\n");
                candidate = process(n->children[i], str);
            }
        }
    }
    if (*(n->end)==-1){
        n->candidate = 0;
        return 0;
    }
    if (candidacy[0] && candidacy[1]){
        candidate = 1;
    }
    if (candidate){
        n->candidate = 1;
    } else{
        n->candidate = 0;
    }
    printf("start: %d, end: %d\ncandidacy: %d\n", *(n->start), *(n->end), candidate);
    return candidate;
}

// Alternative, do processing and lcs both in one
// Why not if it does not work?

int label(node *n){
    if (*(n->end)==-1){
        return 0;
    }
    return *(n->end)-*(n->start)+1;
}

void LCS(node *n, int *lcs, int loc[], int prevLabel){
    if (!n){
        return;
    }
    int root = isRoot(n);
    if (!root){
        if (!n->candidate) return;
        if (isLeaf(n)) return;
    }
    if (label(n)>=*lcs){
        *lcs = prevLabel + label(n);
    }
    prevLabel = label(n);

    for(int i=0; i<MAX_CHAR; i++){
        LCS(n->children[i], lcs, loc, prevLabel);
    }
}

int longestCommonSubstring(suffixTree *st){
    // process the internal nodes
        // candidates will be the ones containing strings from both strings
        // leaves are not candidates, vacuously true, but they are used to determine candidacy
        // of other nodes
    process(st->root, st->str);
    int lcs = 0;
    int loc[2] = {0,0};
    LCS(st->root, &lcs, loc, 0);
    printf("start: %d, end: %d\n", loc[0], loc[1]);
    return lcs;
}
