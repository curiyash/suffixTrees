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
    int partial = 0;
    // printf("start: %d | end: %d\n", start, end);
    for (int j=start; j<=end && pos<strlen(pat); j++){
        // printf("Is %c==%c?\n", pat[pos], str[j]);
        if (pat[pos]!=str[j] && j==start){
            return -1;
        } else if (pat[pos]==str[j]){
            match++;
            *(index) = *(index)+1;
            pos++;
        } else if (pat[pos]!=str[j]){
            partial = 1;
            break;
        }
    }
    // printf("matches: %d, len: %ld\n", match, strlen(pat));
    if (match<strlen(pat)-posStart || partial){
        return 0;
    }
    return 1;
}

void getSuffixIndices(node *n, matchPos *t){
    if (isLeaf(n)){
        pair *p = (pair *) malloc(sizeof(pair));
        p->begin = n->suffixIndex;
        p->next = t->next;
        t->next = p;
    }
    for (int i=0; i<MAX_CHAR; i++){
        if (n->children[i]){
            getSuffixIndices(n->children[i], t);
        }
    }
}

int checkForSubString(suffixTree *st, char *pat){
    node *curr = st->root;
    int index = 0;
    int update = 0;
    int len = strlen(pat);
    int i = 0;
    int finalStatus = -1;
    int partial = 0;
    matchPos *t = (matchPos *) malloc(sizeof(matchPos));
    t->length = len;
    t->next = NULL;
    int wasPartial = 0;
    int beganAt = 0;
    int endedAt = 0;
    int readFrom = 0;
    while (i<MAX_CHAR && index<len){
        // if (i==0){
            // printf("Yes\n");
            // printf("%d %d %d\n", *(curr->start), *(curr->end), index);
        // }
        if (curr->children[i]){
            // If pat doesn't match child label
            // printf("Checking: %d %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
            int status = 0;
            if (wasPartial){
                // printf("Reading from: %d\n", *(curr->children[i]->start)+readFrom);
                status = match(pat, index, *(curr->children[i]->start)+readFrom, *(curr->children[i]->end), st->str, &update);
            } else{
                status = match(pat, index, *(curr->children[i]->start)+readFrom, *(curr->children[i]->end), st->str, &update);
            }
            // printf("status: %d %d\n", status, update);
            finalStatus = status;
            if (status==-1) {
                i++;
                // index = 0;
                if (!wasPartial){
                    index = 0;
                }
                readFrom = 0;
                continue;
            };
            // If full match
            if (status==1){
                // printf("Is a substring\n");
                // i++;
                getSuffixIndices(curr->children[i], t);
                pair *ref = t->next;
                printf("Found at positions...\n");
                while (ref){
                    printf("%d ", ref->begin);
                    ref=ref->next;
                }
                printf("\n");
                return 1;
            }
            // If partial match
            if (status==0){
                // printf("Here %d\n", update);
                if (!wasPartial){
                    beganAt = *(curr->children[i]->start);
                    wasPartial = 1;
                }
                // printf("Partial\n");
                // printf("%d %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
                // printf("update: %d == %d?\n", update, *(curr->children[i]->end) - *(curr->children[i]->end)+1);
                readFrom = update;
                if (update>=*(curr->children[i]->end) - *(curr->children[i]->start)+1){
                    // printf("Child changed\n");
                    curr = curr->children[i];
                    i = 0;
                    readFrom = 0;
                }
                index = update;
                partial = 1;
                continue;
            }
        }
        i++;
    }
    if (finalStatus==1){
        // printf("Is a substring\n");
        // printf("I'm here somehow\n");
        getSuffixIndices(curr, t);
        pair *ref = t->next;
        printf("Found at positions...\n");
        while (ref){
            printf("%d ", ref->begin);
            ref=ref->next;
        }
        printf("\n");
        return 1;
    } else if (partial){
        printf("Detected a partial match at %d\n", beganAt);
        return 0;
    }else{
        printf("Not a substring\n");
        return 0;
    }
}

void displayAllChildren(suffixTree *st, node *curr){
    if (isLeaf(curr)){
        for (int j=*(curr->start); j<=*(curr->end); j++){
            printf("%c", st->str[j]);
        }
        printf("\n");
        return;
    }
    for (int i=0; i<MAX_CHAR; i++){
        if (curr->children[i]){
            if (!isLeaf(curr->children[i])){
                for (int j=*(curr->children[i]->start); j<=*(curr->children[i]->end); j++){
                    printf("%c", st->str[j]);
                }
                printf("|");
            }
            displayAllChildren(st, curr->children[i]);
        }
    }
}

int didYouMean(suffixTree st, char *pat){
    node *curr = st.root;
    int index = 0;
    int update = 0;
    int len = strlen(pat);
    int i = 0;
    int finalStatus = -1;
    int partial = 0;
    int wasPartial = 0;
    int beganAt = 0;
    while (i<MAX_CHAR && index<len){
        if (i==0){
            // printf("Yes\n");
            // printf("%d %d %d\n", *(curr->start), *(curr->end), index);
        }
        if (curr->children[i]){
            // If pat doesn't match child label
            // printf("Checking: %d %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
            int status = match(pat, index, *(curr->children[i]->start), *(curr->children[i]->end), st.str, &update);
            // printf("status: %d %d\n", status, update);
            finalStatus = status;
            if (status==-1) {
                i++;
                index = 0;
                wasPartial = 0;
                continue;
            };
            // If full match
            if (status==1){
                // printf("Is a substring\n");
                return 1;
            }
            // If partial match
            if (status==0){
                if (!wasPartial){
                    beganAt = *(curr->children[i]->start);
                    wasPartial = 1;
                }
                // printf("%d %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
                // printf("update: %d == %d?\n", update, *(curr->children[i]->end) - *(curr->children[i]->end)+1);
                if (update>=*(curr->children[i]->end) - *(curr->children[i]->start)+1){
                    // printf("Child changed\n");
                    curr = curr->children[i];
                    i = 0;
                }
                index = update;
                partial = 1;
                continue;
            }
        }
        i++;
    }
    if (finalStatus==1){
        // printf("Is a substring\n");
        return 1;
    } else if (partial){
        printf("-----------------------------\n");
        printf("start: %d | end: %d\n", *(curr->start), *(curr->end));
        // displayAllChildren(&st, curr);
        printf("Detected a partial match\n");
        printf("-----------------------------\n");
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

int findConsecutiveLeaves(node *n){
    int leaves = 0;
    if (n->suffixIndex!=-1){
        return 1;
    }
    // printf("-----------------------\n");
    for (int i=0; i<MAX_CHAR; i++){
        // if (n->children[i])
            // printf("child start: %d, parent end: %d\n", *(n->children[i]->start), *(n->end));
        if (n->children[i] && *(n->children[i]->start)==*(n->end)+1){
            // printf("Here\n");
            leaves += findConsecutiveLeaves(n->children[i]);
        }
    }
    // printf("-----------------------\n");
    // printf("Leaves: %d\n", leaves);
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
        // if (i==0){
        //     printf("Yes\n");
        //     printf("%d %d %d\n", *(curr->start), *(curr->end), index);
        // }
        if (curr->children[i]){
            // If pat doesn't match child label
            // printf("Checking: %d %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
            int status = match(pat, index, *(curr->children[i]->start), *(curr->children[i]->end), st->str, &update);
            // printf("status: %d %d\n", status, update);
            finalStatus = status;
            if (status==-1) {
                i++;
                continue;
            };
            // If full match
            if (status==1){
                // count = findLeaves(curr->children[i]);
                // printf("start: %d | end: %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
                count = findLeaves(curr->children[i]);
                return count;
            }
            // If partial match
            if (status==0){
                // printf("%d %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
                // printf("update: %d == %d?\n", update, *(curr->children[i]->end) - *(curr->children[i]->end)+1);
                if (update>=*(curr->children[i]->end) - *(curr->children[i]->start)+1){
                    // printf("Child changed\n");
                    curr = curr->children[i];
                    i = 0;
                }
                index = update;
                // curr = curr->children[i];
                // i = 0;
                continue;
            }
        }
        i++;
    }
    if (finalStatus==1){
        // printf("Is a substring\n");
        return count;
    } else{
        printf("Not a substring\n");
        return 0;
    }
}

int countChildren(node *curr){
    int count = 0;
    for (int i=0; i<MAX_CHAR; i++){
        if (curr->children[i]) count++;
    }
    return count;
}

int matchForRepeats(char *pat, int posStart, int start, int end, char *str, int *index){
    // 1 = full match
    // 0 = partial match
    // -1 = no match
    int pos = posStart;
    int match = 0;
    int partial = 0;
    int j = start;
    // printf("start: %d | end: %d\n", start, end);
    // printf("pos: %d | j: %d\n", pos, j);
    for (;j<=end && pos<strlen(pat); j++){
        // printf("Is %c==%c?\n", pat[pos], str[j]);
        if (pat[pos]!=str[j] && j==start){
            return -1;
        } else if (pat[pos]==str[j]){
            match++;
            *(index) = *(index)+1;
            pos++;
        } else if (pat[pos]!=str[j]){
            partial = 1;
            break;
        }
    }
    // printf("end: %d\n", end);
    // printf("index: %d\n", *index);
    // printf("matches: %d, len: %ld\n", match, strlen(pat));
    if (match<strlen(pat)-posStart || partial){
        return 0;
    }
    return 1;
}

int countRepeats(suffixTree *st, char *pat, node *curr, int *count){
    // node *curr = st->root;
    int index = 0;
    int update = 0;
    int len = strlen(pat);
    int i = 0;
    int finalStatus = -1;
    // int count = 0;
    int wasPartial = 0;
    int flagForCheck = 0;
    int readFrom = 0;
    int nodeChange = 0;
    while (i<MAX_CHAR && index<len){
        // if (i==0){
        //     // printf("Yes\n");
        //     // printf("%d %d %d\n", *(curr->start), *(curr->end), index);
        // }
        if (curr->children[i]){
            // If pat doesn't match child label
            printf("Checking: %d %d\n", *(curr->children[i]->start)+readFrom, *(curr->children[i]->end));
            int status = 0;
            update = 0;
            printf("index: %d\n", index);
            if (wasPartial){
                status = match(pat, index, *(curr->children[i]->start)+readFrom, *(curr->children[i]->end), st->str, &update);
            }
            else{
                status = match(pat, index, *(curr->children[i]->start)+readFrom, *(curr->children[i]->end), st->str, &update);
            }
            printf("status: %d %d\n", status, update);
            finalStatus = status;
            if (status==-1) {
                i++;
                if (!wasPartial){
                    index = 0;
                }
                readFrom = 0;
                if (!nodeChange){
                    index=0;
                }
                continue;
            };
            // If full match
            if (status==1){
                // count = findLeaves(curr->children[i]);
                // printf("start: %d | end: %d\n", *(curr->start), *(curr->end));
                // printf("start: %d | end: %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
                // printf("update: %d\n", update);
                *count = *count+1;
                if (isLeaf(curr)){
                    return *count;
                }
                // printf("##############################################\n");
                if (update<*(curr->children[i]->end)-*(curr->children[i]->start)+1){
                    printf("Curr remains\n");
                    // printf("readFrom: %d | end: %d\n", readFrom, *(curr->children[i]->end));
                    printf("index: %d %d %d\n", index, index+update, len);
                    readFrom = readFrom+update;
                    if (index+update==len){
                        index = 0;
                        // readFrom = 0;
                    } else{
                        index = update;
                    }
                    printf("IN index: %d\n", index);
                    printf("readFrom: %d\n", readFrom);
                    nodeChange = 0;
                    // return countRepeats(st, pat, curr, count);
                    continue;
                }
                printf("Curr did not remain\n");
                nodeChange = 1;
                // printf("Node Changed\n");
                return countRepeats(st, pat, curr->children[i], count);
            }
            // If partial match
            if (status==0){
                // printf("Node Changed\n");
                // printf("%d %d\n", *(curr->children[i]->start), *(curr->children[i]->end));
                printf("update: %d == %d?\n", update, *(curr->children[i]->end) - *(curr->children[i]->start)+1);
                printf("index: %d | readFrom: %d\n", index, readFrom);
                if (readFrom+update>=*(curr->children[i]->end) - *(curr->children[i]->start)+1){
                    printf("Child changed\n");
                    nodeChange = 1;
                    curr = curr->children[i];
                    i = 0;
                    readFrom = 0;
                    index = index+update;
                } else{
                    readFrom = readFrom+update;
                    index = 0;
                }
                wasPartial = 1;
                continue;
            }
        }
        i++;
    }
    if (finalStatus==1){
        // printf("Is a substring\n");
        // *count+=1;
        return *count;
    } else{
        // printf("Not a substring\n");
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

void DFS(node *n, int *len, int currPathLen, together *t){
    if (isLeaf(n)){
        return;
    }
    if (!isRoot(n)){
        if (currPathLen + pathLength(n)>*len){
            *len = currPathLen + pathLength(n);
            pair *newP = (pair *) malloc(sizeof(pair));
            newP->end = *(n->end);
            newP->begin = newP->end-*len+1;
            newP->next = NULL;
            t->next = newP;
        } else if (currPathLen + pathLength(n)==*len){
            pair *newP = (pair *) malloc(sizeof(pair));
            newP->end = *(n->end);
            newP->begin = newP->end-*len+1;
            newP->next = t->next;
            t->next = newP;
        }
        currPathLen = currPathLen + pathLength(n);
    }
    // printf("currentPathLen: %d, pathLen: %d\n", currPathLen, pathLength(n));
    // printf("Max len: %d\n", *len);
    for (int i=0; i<MAX_CHAR; i++){
        if (!isLeaf(n->children[i])){
            DFS(n->children[i], len, currPathLen, t);
        }
    }
}

// This is longest repeating substring and not longest repeated substring
int longestRepeatedSubstring(suffixTree *st){
    int len = 0;
    int currPathLen = 0;
    together t;
    t.next = NULL;
    t.lcs = 0;
    // printf("--------------\n");
    // printf("start: %p | end: %p\n", st->root->start, st->root->end);
    DFS(st->root, &len, currPathLen, &t);
    pair *ref = t.next;
    // char *str = (char *) malloc(sizeof(char)*len);
    while (ref){
        for (int i=ref->begin; i<=ref->end; i++){
            // str[i-ref->begin] = st->str[i];
            printf("%c", st->str[i]);
        }
        printf("\n");
        // checkForSubString(st, str);
        ref = ref->next;
    }
    printf("\n");
    return len;
}

int *process(node *n, char *str, int candidacy[2]){
    // printf("start: %d, end: %d\n", *(n->start), *(n->end));
    for (int i=0; i<MAX_CHAR; i++){
        if (n->children[i]){
            if (isLeaf(n->children[i])){
                // printf("Leaf\n");
                if (str[*(n->children[i]->end)]=='$'){
                    // printf("returned $\n");
                    candidacy[0] = 1;
                } else{
                    // printf("returned #\n");
                    candidacy[1] = 1;
                }
            } else {
                // printf("Internal node\n");
                int *candid = (int *) malloc(sizeof(int)*2);
                candid = process(n->children[i], str, candid);
                if (candid[0]){
                    candidacy[0] = 1;
                }
                if (candid[1]){
                    candidacy[1] = 1;
                }
            }
        }
    }
    // printf("candidacy: %d %d\n", candidacy[0], candidacy[1]);
    if (candidacy[0] && candidacy[1]){
        n->candidate = 1;
        return candidacy;
    } else{
        n->candidate = 0;
        return candidacy;
    }
}

int label(node *n){
    if (*(n->end)==-1){
        return 0;
    }
    return *(n->end)-*(n->start)+1;
}

// void LCS(node *n, int *lcs, int rootParent, int prevStart, int prevLabel, int *begin, int *end, char *str){
//     if (!n){
//         return;
//     }
//     int root = isRoot(n);
//     if (rootParent){
//         prevStart = *(n->start);
//         printf("prevStart: %d\n", *(n->start));
//     }
//     if (n->start && n->end){
//         for (int i=*(n->start); i<=*(n->end); i++){
//             printf("%c", str[i]);
//         }
//         printf("\n");
//     }
//     if (!root){
//         rootParent = 0;
//         if (!n->candidate) return;
//         if (isLeaf(n)) return;
//     } else{
//         rootParent = 1;
//     }
//     if (prevLabel+label(n)>=*lcs){
//         printf("Here\n");
//         if (n->end){
//             *end = *(n->end);
//             printf("end: %d\n", *end);
//         }
//         *begin = prevStart;
//         *lcs = prevLabel + label(n);
//     }
//     prevLabel = prevLabel + label(n);

//     for(int i=0; i<MAX_CHAR; i++){
//         // node *n, int *lcs, int root, int prevLabel, int *begin, int *end
//         LCS(n->children[i], lcs, rootParent, prevStart, prevLabel, begin, end, str);
//     }
// }

void LCS(node *n, int prevLabel, int *lcs, int *begin, int *end, int prevStart, together *t){
    if (!n){
        return;
    }
    if (isLeaf(n)) {
        // printf("Leaf\n");
        return;
    }
    if (!n->candidate){
        // printf("Candidacy: 0\n");
        return;
    }
    // printf("prevLabel: %d, label: %d\n", prevLabel, label(n));
    if (prevLabel+label(n) == *lcs) {
        // printf("lcs: %d\n", *lcs);
        *lcs = prevLabel+label(n);
        *end = *(n->end);
        // t->lcs = *lcs;
        pair *newP = (pair *) malloc(sizeof(pair));
        newP->end = *(n->end);
        newP->begin = newP->end-*lcs+1;
        if (t->next){
            newP->next = t->next;
        }
        t->next = newP;
    } else if (prevLabel+label(n) > *lcs){
        *lcs = prevLabel + label(n);
        *end = *(n->end);
        pair *newP = (pair *) malloc(sizeof(pair));
        newP->end = *(n->end);
        newP->begin = newP->end-*lcs+1;
        t->next = newP;
    }
    prevLabel = prevLabel+label(n);
    for(int i=0; i<MAX_CHAR; i++){
        if (n->children[i])
            LCS(n->children[i], prevLabel, lcs, begin, end, prevStart, t);
    }
}

int longestCommonSubstring(suffixTree *st){
    // process the internal nodes
        // candidates will be the ones containing strings from both strings
        // leaves are not candidates, vacuously true, but they are used to determine candidacy
        // of other nodes
    int candidacy[2] = {0, 0};
    process(st->root, st->str, candidacy);
    int lcs = 0;
    int loc[2] = {0,0};
    int prevLabel = 0;
    int begin = 0;
    int end = 0;
    together t;
    t.next = NULL;
    t.lcs = 0;
    for (int i=0; i<MAX_CHAR; i++){
        if (st->root->children[i])
            LCS(st->root->children[i], prevLabel, &lcs, &begin, &end, *(st->root->children[i]->start), &t);
    }
    pair *ref = t.next;
    while (ref){
        // printf("begin: %d, end: %d\n", ref->begin, ref->end);
        for (int i=ref->begin; i<=ref->end; i++){
            printf("%c", st->str[i]);
        }
        printf("\n");
        ref = ref->next;
    }
    return lcs;
}

void preprocessStringForLCS(suffixTree *st, char *str, char *pat){
    int len = strlen(str)-1;
    int other = strlen(pat);
    char *temp = (char *) malloc(sizeof(char)*(len+other+3));
    for (int i=0; i<len; i++){
        temp[i] = str[i];
    }
    // printf("%s\n", temp);
    temp[len] = '#';
    for (int i=len+1; i<len+other+1; i++){
        temp[i] = pat[i-len-1];
    }
    temp[len+other+1] = '$';
    temp[len+other+2] = '\0';
    st->str = (char *) malloc(sizeof(char)*(len+other+3));
    strcpy(st->str, temp);
    free(temp);
}
