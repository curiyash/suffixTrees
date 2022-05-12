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
    st->root = newNode(-1, &(st->end));
    *(st->root->end) = -1;
    st->ap.activeNode = st->root;

    char *str = st->str;

    // Start the loop for each phase
    for (int i=0; i<strlen(str); i++){
        beginPhase(st, i);
    }
}

void walkDown(suffixTree *st, char c){
    // Travel until active
    node *n = st->ap.activeNode->children[c];
    int start = *(n->start);
    int end = *(n->end);

    if ((end-start)<st->ap.activeLength){
        st->ap.activeNode = n;
        st->ap.activeLength = st->ap.activeLength - (end-start);
        st->ap.activeEdge = *(n->children[c]->start);
    } else{
        st->ap.activeLength++;
    }
}

void beginPhase(suffixTree *st, int i){
    // In each phase, 'remaining' and 'end' should be incremented
    st->remaining += 1;
    // Rule 1 extension
    st->end += 1;
    printf("End: %d\n", st->end);
    char c = st->str[i];
    printf("Char: %c\n", c);

    // Handle suffixLinks
    node *lastCreatedInternalNode = NULL;

    while (st->remaining>0){
        // Check active length
        if (st->ap.activeLength==0){
            // Is there a path from activeNode with i?

            // Create a path if not
            if (st->ap.activeNode->children[c]==NULL){
                printf("1a\n");
                node *nn = newNode(i, &(st->end));
                st->ap.activeNode->children[c] = nn;
                st->remaining--;
            }
            else {
                // Show stopper
                // activeIndex becomes start index of the path that exists
                printf("2a\n");
                st->ap.activeEdge = *(st->root->children[c]->start);
                st->ap.activeLength++;
                break;
            }
        } else {
            // Move activeLengths along the direction given by activeEdge
            // from activeNode
            int moveBy = st->ap.activeLength;
            int moveTowards = st->ap.activeEdge;
            node *moveFrom = st->ap.activeNode;
            int movement = moveTowards+moveBy;
            // Is that character current character?

            if (st->str[movement]==c){
                // Rule 3 extension
                // Show stopper
                // End phase
                if (lastCreatedInternalNode){
                    lastCreatedInternalNode->suffixLink = st->ap.activeNode;
                }
                // Need to create walkdown function
                walkDown(st, c);
                break;
            } else{
                // New internal node needs to be created
                // So there is an edge from activeNode to the new
                // internal node with [start, end] = [activeEdge, activeLength-1]
                // Then, we have branches from this internal node
                // A branch exists for activeLength
                // A new branch is created for the current character
                // node *new = (node *) malloc(sizeof(node));
                node *new = newNode(st->ap.activeEdge, NULL);
                new->end = (int *) malloc(sizeof(int));
                *(new->end) = st->ap.activeLength;
                printf("new\n");
                printf("start: %d, end; %d\n", *(new->start), *(new->end));
                // *(new->start) = st->ap.activeEdge;
                // *(new->end) = st->ap.activeLength;
                free(st->ap.activeNode->children[st->str[st->ap.activeEdge]]);
                st->ap.activeNode->children[st->str[st->ap.activeEdge]] = new;

                node *nn = newNode(st->ap.activeLength, &(st->end));
                // How to represent the leaf node?
                // Possibly moveBy instead of activeLength
                printf("start: %d, end; %d\n", *(nn->start), *(nn->end));
                char next = st->str[moveBy];
                new->children[next] = nn;

                node *n = newNode(i, &(st->end));
                new->children[c] = n;

                // Every internal node has a suffix link
                // need to think about suffix links
                new->suffixLink = st->root;
                if (lastCreatedInternalNode){
                    lastCreatedInternalNode->suffixLink = new;
                }
                lastCreatedInternalNode = new;

                st->remaining--;
                st->ap.activeNode = new->suffixLink;
                st->ap.activeLength--;
                st->ap.activeEdge++;
            }
        }
    }
}

void Display(node *t){
    if (t){
        printf("%d %d\n", *(t->start), *(t->end));
        for (int i=0; i<MAX_CHAR; i++){
            Display(t->children[i]);
        }
    }
}
