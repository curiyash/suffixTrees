#include <stdio.h>
#include <stdlib.h>
#include "suffixTree.h"
#include <string.h>

// Define a unique char which avoid implicit suffix trees
#define UNIQUE_CHAR '$'

// Functions
    // Defined only in this file
void beginPhase(suffixTree *st, int i);

// 1. initSuffixTree
    // Input: 
        // suffixTree *st - pointer to a suffix tree
    // Expected outcome:
        // Initialize all the properties of tree
        // Point root to NULL
        // Initialize remaining and end
        // Intialize active points
void initSuffixTree(suffixTree *st){
    st->root = NULL;
    st->remaining = 0;
    st->end = -1;
    st->ap.activeNode = NULL;
    st->ap.activeEdge = -1;
    st->ap.activeLength = 0;
}

int edgeLength(node *n){
    return *(n->end)-*(n->start)+1;
}

void print(int start, int end, char *str){
    for (int i=start; i<end; i++){
        printf("%c", str[i]);
    }
    printf("\n");
}

void setSuffixIndexByDFS(node *n, int labelHeight, char *str)
{
    if (n == NULL) return;
 
    // if (*(n->start) != -1) //A non-root node
    // {
    //     //Print the label on edge from parent to current node
    //     print(*(n->start), *(n->end), str);
    // }
    int leaf = 1;
    int i;
    for (i = 0; i < MAX_CHAR; i++)
    {
        if (n->children[i] != NULL)
        {
            if (leaf == 1 && *(n->start) != -1)
                // printf(" [%d]\n", n->suffixIndex);
 
            //Current node is not a leaf as it has outgoing
            //edges from it.
            leaf = 0;
            setSuffixIndexByDFS(n->children[i],
                  labelHeight + edgeLength(n->children[i]), str);
        }
    }
    if (leaf == 1)
    {
        for(i = *(n->start); i<= *(n->end); i++)
        {
            if(str[i] == '#') //Trim unwanted characters
            {
                n->end = (int*) malloc(sizeof(int));
                *(n->end) = i;
                break;
            }
        }
        n->suffixIndex = strlen(str) - labelHeight;
        // printf(" %d\n", n->suffixIndex);
    }
}

// 2. preprocessString
    // Input:
        // suffixTree *st - pointer to a suffixTree
        // char *str - string for which to create suffixTree
    // Expected outcome:
        // Add unique character to string at last
        // Initialize suffixTree's string variable
void preprocessString(suffixTree *st, char *str){
    int lengthStr = strlen(str);
    st->str = (char *) malloc(sizeof(char)*lengthStr+1);
    if (!st->str){
        printf("Out of memory\n");
    }
    memcpy(st->str, str, lengthStr);
    st->str[lengthStr] = UNIQUE_CHAR;
}

node *newNode(int start, int *end){
    node *nn = (node *) malloc(sizeof(node));

    for (int i=0; i<MAX_CHAR; i++){
        nn->children[i] = NULL;
    }
    nn->start = (int *) malloc(sizeof(int));
    nn->end = (int *) malloc(sizeof(int));
    *(nn->start) = start;
    nn->end = end;
    nn->suffixLink = NULL;
    return nn;
}

// 3. buildSuffixTree
    // Input:
        // suffixTree *st: pointer to a suffixTree
    // Expected outcome:
        // Build the goddamn suffixTree for its string
void buildSuffixTree(suffixTree *st){
    st->root = newNode(-1, NULL);
    st->root->end = (int *) malloc(sizeof(int));
    *(st->root->end) = -1;
    st->ap.activeNode = st->root;

    char *str = st->str;

    // Start the loop for each phase
    for (int i=0; i<strlen(str); i++){
        beginPhase(st, i);
    }

    // Set suffix indices
    setSuffixIndexByDFS(st->root, 0,st->str);
}

void walkDown(suffixTree *st, int i){
    // Travel until active
    node *n = st->ap.activeNode->children[st->str[st->ap.activeEdge]];
    int start = *(n->start);
    int end = *(n->end);
    // printf("walkDown start: %d\n", start);
    // printf("walkDown end: %d\n", end);
    // if (st->str[i]=='z')
        // printf("Here in function %c %d\n", st->str[i], *(st->root->children[st->str[0]]->children[st->str[i]]->start));

    if ((end-start)<st->ap.activeLength){
        st->ap.activeNode = n;
        st->ap.activeLength = st->ap.activeLength - (end-start);
        printf("%p\n", st->ap.activeNode->children[st->str[i]]->start);
        st->ap.activeEdge = *(st->ap.activeNode->children[st->str[i]]->start);
    printf("Here\n");
        // printf("Here\n");
    } else{
        // printf("Here in else\n");
        st->ap.activeLength++;
    }
}

void printActivePoints(suffixTree st, activePoint ap){
    printf("remaining: %d\n", st.remaining);
    printf("end: %d\n", st.end);
    if (ap.activeNode){
        printf("activeNode\nstart: %d, end: %d\n", *(ap.activeNode->start), *(ap.activeNode->end));
    }
    printf("activeEdge: %d\n", ap.activeEdge);
    printf("activeLength: %d\n", ap.activeLength);
}

void beginPhase(suffixTree *st, int i){
    // In each phase, 'remaining' and 'end' should be incremented
    st->remaining += 1;
    // Rule 1 extension
    st->end += 1;
    // printf("End: %d\n", st->end);
    char c = st->str[i];
    // printf("Char: %c\n", c);

    // Handle suffixLinks
    node *lastCreatedInternalNode = NULL;

    while (st->remaining>0){
        // Check active length
        if (st->ap.activeLength==0){
            // Is there a path from activeNode with i?

            // Create a path if not
            printf("1\n");
            if (st->ap.activeNode->children[c]==NULL){
                printf("1a\n");
                node *nn = newNode(i, &(st->end));
                // printf("nn\nstart: %d, end; %d\n", *(nn->start), *(nn->end));
                st->ap.activeNode->children[c] = nn;
                st->remaining--;
            }
            else {
                // Show stopper
                // activeIndex becomes start index of the path that exists
                printf("1b\n");
                st->ap.activeEdge = *(st->root->children[c]->start);
                st->ap.activeLength++;
                break;
            }
        } else {
            printf("2\n");
            // Move activeLengths along the direction given by activeEdge
            // from activeNode
            int moveBy = st->ap.activeLength;
            int moveTowards = st->ap.activeEdge;
            node *moveFrom = st->ap.activeNode->children[st->str[st->ap.activeEdge]];
            int movement = moveTowards+moveBy;
            // Is that character current character?
            printf("movement: %d, edgeLength: %d\n", movement, edgeLength(moveFrom)-1);
            if (movement>=*(moveFrom->end)-1){
                // End of path
                printf("End of path\n");
            };

            if (st->str[movement]==c){
                printf("2a\n");
                // Rule 3 extension
                // Show stopper
                // End phase
                if (lastCreatedInternalNode){
                    // printf("2x\n");
                    lastCreatedInternalNode->suffixLink = st->ap.activeNode;
                }
                // Need to create walkdown function
                walkDown(st, i);
                break;
            } else{
                printf("2b\n");
                // New internal node needs to be created
                // So there is an edge from activeNode to the new
                // internal node with [start, end] = [activeEdge, activeLength-1]
                // Then, we have branches from this internal node
                // A branch exists for activeLength
                // A new branch is created for the current character
                // node *new = (node *) malloc(sizeof(node));
                printf("activeEdge: %d\n", st->ap.activeEdge);
                node *old = st->ap.activeNode->children[st->str[st->ap.activeEdge]];
                int oldStart = *(old->start);
                // printf("42\n");
                // printf("-----\n");
                // printActivePoints(*st, st->ap);
                // printf("-----\n");
                *(old->start) = *(old->start)+st->ap.activeLength;
                // printf("old\nstart: %d, end: %d\n", *(old->start), *(old->end));
                node *new = newNode(oldStart, NULL);
                new->end = (int *) malloc(sizeof(int));
                *(new->end) = oldStart+st->ap.activeLength-1;
                // printf("new\nstart: %d, end; %d\n", *(new->start), *(new->end));
                // *(new->start) = st->ap.activeEdge;
                // *(new->end) = st->ap.activeLength;
                // printf("new\nstart: %d, end; %d\n", *(new->start), *(new->end));

                // node *nn = newNode(*(new->end)+1, &(st->end));
                // // How to represent the leaf node?
                // // Possibly moveBy instead of activeLength
                // printf("nn\nstart: %d, end; %d\n", *(nn->start), *(nn->end));
                // char next = st->str[moveBy];
                // new->children[next] = nn;


                node *n = newNode(i, &(st->end));
                new->children[st->str[*(new->start)+st->ap.activeLength]] = old;
                new->children[c] = n;
                // printf("n\ni: %d, start: %d, end; %d\n", i, *(n->start), *(n->end));
                // printf("char: %c\n", st->str[oldStart]);
                // free(st->ap.activeNode->children[st->str[oldStart]]);
                st->ap.activeNode->children[st->str[oldStart]] = new;

                // Every internal node has a suffix link
                // need to think about suffix links
                if (lastCreatedInternalNode){
                    // printf("New suffix link\n");
                    lastCreatedInternalNode->suffixLink = new;
                }
                lastCreatedInternalNode = new;
                new->suffixLink = st->root;

                st->remaining--;
                if (st->ap.activeNode!=st->root){
                    st->ap.activeNode = st->ap.activeNode->suffixLink;
                } else{
                    st->ap.activeEdge++;
                    st->ap.activeLength--;
                }
            }
        }
        // printf("-----\n");
        // printActivePoints(*st, st->ap);
        // printf("-----\n");
    }
    // printf("-----\n");
    // printActivePoints(*st, st->ap);
    // printf("-----\n");
}

int count = 0;

void Display(node *t, char *str){
    if (t){
        // printf("%d %d\n", *(t->start), *(t->end));
        for (int i=*(t->start); i<*(t->end)+1; i++){
            printf("%c", str[i]);
        }
        printf("\n");
        int flag = 0;
        for (int i=0; i<MAX_CHAR; i++){
            if (!t->children[i]) flag++;
            Display(t->children[i], str);
        }
        if (flag==256){
            count++;
            printf("Leaf. Suffix index: %d\n", t->suffixIndex);
        }
        printf("countLeaf: %d\n", count);
    }
}

// int checkForSubString(suffixTree st, char *pat){
//     int pos = 0;
//     node *ref = st.root;
//     char cmp = ' ';
//     int cmpIndex = 0;
//     while (pos<strlen(pat)){
//         printf("pat char: %c\n", pat[pos]);
//         printf("%d %d %d\n", pos, *(ref->start), *(ref->end));
//         if (*(ref->end)!=-1 && pos<edgeLength(ref)){
//             printf("1\n");
//             cmp = *(ref->start)+cmpIndex;
//             printf("%c %c\n", pat[pos], st.str[cmp]);
//             if (pat[pos]==st.str[cmp]){
//                 cmpIndex++;
//             }
//             else{
//                 printf("Not a substring\n");
//                 return 0;
//             }
//             pos++;
//         } else if (ref->children[pat[pos]]){
//             printf("2\n");
//             ref = ref->children[pat[pos]];
//             cmpIndex = 1;
//             pos++;
//         } else{
//             printf("%d %d %d\n", *(ref->start), *(ref->end), edgeLength(ref));
//             printf("Not a substring\n");
//             return 0;
//         }
//     }
//     printf("Is a substring\n");
//     return 1;
// }

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
    printf("Here\n");
    int index = 0;
    int update = 0;
    int len = strlen(pat);
    int i = 0;
    int finalStatus = -1;
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
                continue;
            }
        }
        i++;
    }
    if (finalStatus==1){
        printf("Is a substring\n");
        return 1;
    } else{
        printf("Not a substring\n");
        return 0;
    }
}

int traverseEdge(char *str, int idx, int start, int end, char *text)
{
    int k = 0;
    //Traverse the edge with character by character matching
    for(k=start; k<=end && str[idx] != '\0'; k++, idx++)
    {
        if(text[k] != str[idx])
            return -1;  // mo match
    }
    if(str[idx] == '\0')
        return 1;  // match
    return 0;  // more characters yet to match
}
 
int doTraversal(node *n, char* str, int idx, char *text)
{
    if(n == NULL)
    {
        return -1; // no match
    }
    int res = -1;
    //If node n is not root node, then traverse edge
    //from node n's parent to node n.
    if(*(n->start) != -1)
    {
        res = traverseEdge(str, idx, *(n->start), *(n->end), text);
        if(res != 0)
            return res;  // match (res = 1) or no match (res = -1)
    }
    //Get the character index to search
    idx = idx + edgeLength(n);
    //If there is an edge from node n going out
    //with current character str[idx], traverse that edge
    if(n->children[str[idx]] != NULL)
        return doTraversal(n->children[str[idx]], str, idx, text);
    else
        return -1;  // no match
}
 
// int checkForSubString(suffixTree st, char *pat)
// {
//     int res = doTraversal(st.root, pat, 0, st.str);
//     if(res == 1){
//         printf("Pattern <%s> is a Substring\n", pat);
//         return 1;
//     }
//     else{
//         printf("Pattern <%s> is NOT a Substring\n", pat);
//         return 0;
//     }
// }
