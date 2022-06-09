#include <stdio.h>
#include <stdlib.h>
#include "suffixTree.h"
#include "utilities.h"

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

int main(int argc, char *argv[]){
    suffixTree st;
    initSuffixTree(&st);
    // int maxLength = maxLen(argc, argv);
    // printf("maxLength: %d\n", maxLength);
    // printf("maxLength: %d\n", maxLength);
    // printf("---\n");
    // char **arr = (char **) malloc(sizeof(char *)*atoi(argv[2]));
    // get_me_random_n_words(argc, argv, maxLength, arr);
    // displaySelectedWords(arr, atoi(argv[2]));
    // printf("%p %s\n", arr, arr[0]);
    // printf("---\n");
    // printf("Here\n");
    char *str = "AAGGTAAGTTTAGAATATAAAAGGTGAGTTAAATAGAATAGGTTAAAATTAAAGGAGATCAGATCAGATCAGATCTATCTATCTATCTATCTATCAGAAAAGAGTAAATAGTTAAAGAGTAAGATATTGAATTAATGGAAAATATTGTTGGGGAAAGGAGGGATAGAAGG";
    preprocessString(&st, str);
    printf("%s\n", st.str);
    buildSuffixTree(&st);
    // Display(st.root, st.str);
    // printf("---------------------------------------------------------\n");
    // // for (int i=0; i<256; i++){
    // //     if (st.root->children[i]){
    // //         printf("%c\n", i);
    // //         printf("%d %d\n", *(st.root->children[i]->start), *(st.root->children[i]->end));
    // //     }
    // // }
    // // checkForSubString(st, "ip");
    // // checkForSubString(st, "SSI");
    // // int count = countOccurences(&st, "AGATC");
    // // printf("count: %d\n", count);
    printf("longest repeated substring: %d\n", longestRepeatedSubstring(&st));
    // printf("longest common substring: %d\n", longestCommonSubstring(&st));
    // // printRoot2Leaf(&st);

    // // check(st.root, st.str);
    // printf("\n");
}
