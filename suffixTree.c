#include <stdio.h>
#include <stdlib.h>
#include "suffixTree.h"
#include <string.h>

// Define a unique char which avoids implicit suffix trees
#define UNIQUE_CHAR '$'
int MAX_CHAR = 256;
int STATUS = 0;

// Functions
    // Defined only in this file
void beginPhase(suffixTree *st, int i, int *numNodes, int *memFlag);

int freeSuffixTree(node *n, int *count){
    if (n){
        node *ref = n;
        for (int i=0; i<MAX_CHAR; i++){
            // Do DFS traversal
            // Once all children are visited, delete the parent
            if (n->children[i]){
                freeSuffixTree(n->children[i], count);
            }
        }
        *count = *count+1;
        free(n->children);
        free(n);
    }
}

int Load(suffixTree *st, int loaded){
    char filename[100];
    int goAhead = 0;
    int freeNodes = 0;
    char c;
    if (loaded){
        printf("A suffix tree was already loaded for the string - %s\n Do you want to delete it? 1/0\n", st->str);
        fflush(stdin);
        scanf("%d", &goAhead);
        fflush(stdin);
        switch(goAhead){
            case 1: freeSuffixTree(st->root, &freeNodes);
                    printf("Freed %d nodes\n", freeNodes);
                    break;
            case 0: return 1;
            default: printf("Invalid input\n");
                printf("Aborting operation\n");
                return 1;
        }
    } else{
        loaded = 1;
    }
    int numNodes = 0;
    printf("Enter path of file to read-in a sequence\nNote: filename is limited to be under 100 characters\n");
    scanf(" %[^\n]%*c", filename);
    FILE *fp = fopen(filename, "r");
    // printf("%s\n", filename);
    if (!fp){
        printf("Error in opening a file. Possibly look at...\n1. If you have entered correct path\n2. If the path actually exists\n");
        loaded = 0;
        return 0;
    }
    int count = 0;
    for (c=getc(fp); c!=EOF; c=getc(fp)){
        if (c=='\n' || c=='\r' || c=='\t') {
            count--;
        }
        count++;
    }
    char *str = (char *) malloc(sizeof(char)*(count+1));
    rewind(fp);
    fscanf(fp, "%[^\n]%*c", str);
    // printf("%s\n", str);
    preprocessString(st, str);
    numNodes = buildSuffixTree(st);
    if (numNodes==-1){
        printf("Out of memory :|\nIf you would please, report this to me at: curiyash19@gmail.com with the string input you were trying. I'll try and get back to you with possible hopes of resolving the issue :)\n");
        loaded = 0;
        return 0;
    } else{
        printf("\nSuccessfully built the tree\n");
        printf("number of nodes: %d\n\n", numNodes);
        return 1;
    }
}

int LoadPat(char **pat){
    char filename[100];
    char c;
    printf("Enter path of file to read-in the pattern\nNote: filename is limited to be under 100 characters\n");
    scanf(" %[^\n]%*c", filename);
    FILE *fp = fopen(filename, "r");
    // printf("%s\n", filename);
    if (!fp){
        printf("Error in opening a file. Possibly look at...\n1. If you have entered correct path\n2. If the path actually exists\n3. Do not include '\\' as a separator for path, use '/'\n");
        return 1;
    }
    int count = 0;
    for (c=getc(fp); c!=EOF; c=getc(fp)){
        if (c=='\n' || c=='\r' || c=='\t') {
            count--;
        }
        count++;
    }
    *pat = (char *) malloc(sizeof(char)*(count+1));
    if (!(*pat)){
        return 1;
    }
    rewind(fp);
    fscanf(fp, "%[^\n]%*c", *pat);
    // printf("%s\n", *pat);
    return 0;
}

// 1. initSuffixTree
    // Input: 
        // suffixTree *st - pointer to a suffix tree
    // Expected outcome:
        // Initialize all the properties of tree
        // Point root to NULL
        // Initialize remaining and end
        // Intialize active points
void initSuffixTree(suffixTree *st, int maxChar, int status){
    st->root = NULL;
    st->remaining = 0;
    st->end = -1;
    st->ap.activeNode = NULL;
    st->ap.activeEdge = -1;
    st->ap.activeLength = 0;
    MAX_CHAR = maxChar;
    STATUS = status;
}

int Decoder(char C){
    switch(C){
        case '\0': return 0;
        case '#': return 1;
        case '$': return 2;
        case 'A': return 3;
        case 'C': return 4;
        case 'G': return 5;
        case 'T': return 6;
        default: printf("Invalid character encountered\n"); return -1;
    }
}

int Decode(char c){
    // printf("STATUS: %d\n", STATUS);
    if (!STATUS){
        return c;
    } else{
        return Decoder(c);
    }
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
            // if (leaf == 1 && *(n->start) != -1)
                // printf(" [%d]\n", n->suffixIndex);
 
            //Current node is not a leaf as it has outgoing
            //edges from it.
            leaf = 0;
            setSuffixIndexByDFS(n->children[i],labelHeight + edgeLength(n->children[i]), str);
        }
    }
    if (leaf == 1)
    {
        for(i = *(n->start); i<= *(n->end); i++)
        {
            // Trim unwanted characters
            if(str[i] == '#')
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
    long int lengthStr = strlen(str);
    // printf("length: %ld\n", lengthStr);
    st->str = (char *) malloc(sizeof(char)*(lengthStr+2));
    if (!st->str){
        printf("Out of memory\n");
    }
    strcpy(st->str, str);
    int i=0;
    st->str[lengthStr] = UNIQUE_CHAR;
    st->str[lengthStr+1] = '\0';
    // while (st->str[i]){
    //     printf("%c", st->str[i]);
    //     i++;
    // }
    // printf("\n");
}

node *newNode(int start, int *end){
    node *nn = (node *) malloc(sizeof(node));
    if (!nn){
        return NULL;
    }
    nn->children = (node **) malloc(sizeof(node *)*MAX_CHAR);

    for (int i=0; i<MAX_CHAR; i++){
        nn->children[i] = NULL;
    }
    nn->start = (int *) malloc(sizeof(int));
    if (!nn->start){
        return NULL;
    }
    *(nn->start) = start;
    nn->end = end;
    nn->suffixLink = NULL;
    // printf("MAX_CHAR: %d\n", MAX_CHAR);
    return nn;
}

// 3. buildSuffixTree
    // Input:
        // suffixTree *st: pointer to a suffixTree
    // Expected outcome:
        // Build the suffixTree for its string
    // Return: number of nodes
            // -1: heap overflow
int buildSuffixTree(suffixTree *st){
    int numNodes = 0;
    st->root = newNode(-1, NULL);
    if (!st->root){
        return -1;
    }
    numNodes++;
    // printf("Here42\n");
    st->root->end = (int *) malloc(sizeof(int));
    *(st->root->end) = -1;
    st->ap.activeNode = st->root;

    char *str = st->str;
    // printf("%s\n", str);
    // printf("Here\n");

    // Start the loop for each phase
    // printf("strlen(str): %ld\n", strlen(str));
    int memFlag = 0;
    char status;
    for (int i=0; i<strlen(str); i++){
        beginPhase(st, i, &numNodes, &memFlag);
        if (memFlag){
            return -1;
        }
        // printf("numNodes: %d\n", numNodes);
    }

    // Set suffix indices
    setSuffixIndexByDFS(st->root, 0,st->str);
    return numNodes;
}

int diff(node *n){
    return *(n->end)-*(n->start);
}

void walkDown(suffixTree *st, int i){
    // Travel until active
    node *n = st->ap.activeNode->children[Decode(st->str[st->ap.activeEdge])];

    if (diff(n)<st->ap.activeLength){
        st->ap.activeNode = n;
        st->ap.activeLength = st->ap.activeLength - diff(n);
        st->ap.activeEdge = *(st->ap.activeNode->children[Decode(st->str[i])]->start);
    } else{
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

int isEndOfPath(suffixTree *st, int i){
    node *n = st->ap.activeNode->children[Decode(st->str[st->ap.activeEdge])];
    char *input = st->str;
    if (diff(n)>=st->ap.activeLength){
        return Decode(input[*(st->ap.activeNode->children[Decode(input[st->ap.activeEdge])]->start)+st->ap.activeLength]);
    }
    if (diff(n)+1==st->ap.activeLength){
        if(n->children[Decode(input[i])]){
            return Decode(input[i]);
        }
    } else{
        st->ap.activeNode = n;
        st->ap.activeLength = st->ap.activeLength-diff(n)-1;
        st->ap.activeEdge = st->ap.activeEdge + diff(n) + 1;
        return isEndOfPath(st, i);
    }

    return 0;
}

void beginPhase(suffixTree *st, int i, int *numNodes, int *memFlag){
    // In each phase, 'remaining' and 'end' should be incremented
    st->remaining += 1;
    // Rule 1 extension
    st->end += 1;
    // printf("End: %d\n", st->end);
    // char c = Decode(st->str[i]);
    // printf("str: %c\n", st->str[i]);
    // int dec = Decode(st->str[i]);
    // printf("dec: %d\n", dec);
    // printf("c: %c\n", c);
    // printf("Char: %c\n", c);
    // Handle suffixLinks
    node *lastCreatedInternalNode = NULL;

    while (st->remaining>0){
        // Check active length
        if (st->ap.activeLength==0){
            // Is there a path from activeNode with i?

            // Create a path if not
            // printf("1\n");
            if (st->ap.activeNode->children[Decode(st->str[i])]==NULL){
                // printf("1a\n");
                node *nn = newNode(i, &(st->end));
                if (!nn){
                    *memFlag = 1;
                    break;
                }
                *numNodes = *numNodes+1;
                // printf("nn\nstart: %d, end; %d\n", *(nn->start), *(nn->end));
                // st->ap.activeNode->children[c] = nn;
                st->root->children[Decode(st->str[i])] = nn;
                st->remaining--;
            }
            else {
                // Show stopper
                // activeIndex becomes start index of the path that exists
                // printf("1b\n");
                st->ap.activeEdge = *(st->root->children[Decode(st->str[i])]->start);
                st->ap.activeLength++;
                break;
            }
        } else {
            // printf("Here\n");
            int ch = isEndOfPath(st, i);
            if(ch!=0){
                // printf("2a\n");
                if (ch==Decode(st->str[i])){
                    // printf("eq\n");
                    if (lastCreatedInternalNode){
                        lastCreatedInternalNode->suffixLink = st->ap.activeNode->children[Decode(st->str[st->ap.activeEdge])];
                    }
                    walkDown(st, i);
                    break;
                } else{
                    node *n = st->ap.activeNode->children[Decode(st->str[st->ap.activeEdge])];
                    int oldStart = *(n->start);
                    *(n->start) = oldStart + st->ap.activeLength;

                    node *new = newNode(oldStart, NULL);
                    if (!new){
                        *memFlag = 1;
                        break;
                    }
                    *numNodes = *numNodes+1;
                    new->end = (int *) malloc(sizeof(int));
                    *(new->end) = oldStart+st->ap.activeLength-1;
                    // printf("neq\n");

                    node *newLeaf = newNode(i, &(st->end));
                    if (!newLeaf){
                        *memFlag = 1;
                        break;
                    }
                    *numNodes = *numNodes+1;

                    new->children[Decode(st->str[*(new->start)+st->ap.activeLength])] = n;
                    new->children[Decode(st->str[i])] = newLeaf;
                    new->suffixIndex = -1;
                    st->ap.activeNode->children[Decode(st->str[*(new->start)])] = new;

                    if (lastCreatedInternalNode){
                        lastCreatedInternalNode->suffixLink = new;
                    }
                    lastCreatedInternalNode = new;
                    new->suffixLink = st->root;

                    if (st->ap.activeNode!=st->root){
                        st->ap.activeNode = st->ap.activeNode->suffixLink;
                    } else{
                        st->ap.activeEdge = st->ap.activeEdge+1;
                        st->ap.activeLength--;
                    }
                    st->remaining--;
                }
            } else{
                // printf("2b\n");
                // End of Path
                // printf("End of path\n");
                node *n = st->ap.activeNode->children[Decode(st->str[st->ap.activeEdge])];
                n->children[Decode(st->str[i])] = newNode(i, &(st->end));
                if (!n->children[Decode(st->str[i])]){
                    *memFlag = 1;
                    break;
                }
                *numNodes = *numNodes+1;
                if (lastCreatedInternalNode){
                    lastCreatedInternalNode->suffixLink = n;
                }
                lastCreatedInternalNode = n;
                if (st->ap.activeNode != st->root){
                    st->ap.activeNode = st->ap.activeNode->suffixLink;
                } else{
                    st->ap.activeEdge = st->ap.activeEdge + 1;
                    st->ap.activeLength--;
                }
                st->remaining--;
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
            // Display(t->children[i], str);
        }
        if (flag==MAX_CHAR){
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

int isLeaf(node *t){
    if (t){
        if (*(t->end)==-1){
            return 0;
        }
        if (t->suffixIndex!=-1){
            return 1;
        } else{
            return 0;
        }
    } else{
        return 1;
    }
}

void DFSPath(node *n, char *prevSuffix, char *text){
    char *currSuffix = (char *) malloc(sizeof(char)*(edgeLength(n)));
    // printf("str: %s\n", text);
    // printf("start: %d, end: %d\n", *(n->start), *(n->end));
    for (int i=*(n->start); i<=*(n->end); i++){
        // printf("%c ", text[i]);
        currSuffix[i-*(n->start)] = text[i];
    }
    // printf("\nprevSuffix: %s\n", prevSuffix);
    // printf("currSuffix: %s\n", currSuffix);
    char *newSuffix = (char *) malloc(sizeof(char)*(strlen(prevSuffix) + strlen(currSuffix)));
    strcat(newSuffix, prevSuffix);
    strcat(newSuffix, currSuffix);
    if (*(n->end)!=-1){
        if (isLeaf(n)){
            // printf("%s\n", newSuffix);
            return;
        }
    } else{
        newSuffix = "";
    }
    for (int i=0; i<MAX_CHAR; i++){
        if (n->children[i]){
            DFSPath(n->children[i], newSuffix, text);
        }
    }
}

void printRoot2Leaf(suffixTree *st){
    char *str = (char *) malloc(sizeof(char)*(strlen(st->str)+1));
    DFSPath(st->root, str, st->str);
}
