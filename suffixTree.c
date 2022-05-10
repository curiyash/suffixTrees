#include <stdio.h>
#include <stdlib.h>
#include "suffixTree.h"
#include <string.h>

// Define a unique char which avoid implicit suffix trees
#define UNIQUE_CHAR '$'

// Functions
    // Defined only in this file
void beginPhase(suffixTree *st, node *t);

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
    st->root->start = -1;
    st->root->end = -1;
    st->ap.activeNode = NULL;
    st->ap.activeEdge = 0;
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
    st->str[lengthStr+1] = UNIQUE_CHAR;
}

// 3. buildSuffixTree
    // Input:
        // suffixTree *st: pointer to a suffixTree
    // Expected outcome:
        // Build the goddamn suffixTree for its string
void buildSuffixTree(suffixTree *st){
    st->root = (node *) malloc(sizeof(node));
    st->root->suffixLink = NULL;

    char *str = st->str;

    // Start the loop for each phase
    for (int i=0; i<strlen(str); i++){
        beginPhase(st, i);
    }
}

void beginPhase(suffixTree *st, int i){
    // In each phase, 'remaining' and 'end' should be incremented
    st->remaining += 1;
    // Rule 1 extension
    st->end += 1;
    char c = st->str[i];

    // Handle suffixLinks
    node *lastCreatedInternalNode = NULL;

    while (remaining>0){
        // Check active length
        if (st->ap.activeLength==0){
            // Is there a path from activeNode with i?

            // Create a path if not
            if (st->ap.activeNode->children[c]==NULL){
                node *nn = (node *) malloc(sizeof(node));
                // How to represent the leaf node?
                *(nn->start) = i;
                nn->end = &(st->end);
                st->ap.activeNode->children[c] = nn;
                remaining--;
            }
            else {
                // Show stopper
                st->ap.activeEdge = st->root->children[c]->start;
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
                // End phase;
                if (lastCreatedInternalNode){
                    lastCreatedInternalNode->suffixLink = st->ap.activeNode;
                }
                // Need to create walkdown function
                st->ap.activeLength++;
                break;
            } else{
                // New internal node needs to be created
                // So there is an edge from activeNode to the new
                // internal node with [start, end] = [activeEdge, activeLength-1]
                // Then, we have branches from this internal node
                // A branch exists for activeLength
                // A new branch is created for the current character
                node *new = (node *) malloc(sizeof(node));
                *(new->start) = activeEdge;
                *(new->end) = activeLength;
                free(st->ap.activeNode->children[activeEdge]);
                st->ap.activeNode->children[activeEdge] = new;

                node *nn = (node *) malloc(sizeof(node));
                // How to represent the leaf node?
                *(nn->start) = activeLength;
                nn->end = &(st->end);
                // Possibly moveBy instead of activeLength
                char *next = st->str[moveBy];
                new->children[next] = nn;

                node *n = (node *) malloc(sizeof(node));
                *(n->start) = i;
                n->end = &(st->end);
                new->children[c] = n;

                // Every internal node has a suffix link
                // need to think about suffix links
                new->suffixLink = prevLink;

                remaining--;
                st->ap.activeLength--;
                st->ap.activeEdge++;
            }
        }
    }
}
