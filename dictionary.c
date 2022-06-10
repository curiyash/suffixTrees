#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void makeString(char *arr[], int n, char **str, int maxLen){
    *str = (char *) malloc(sizeof(char)*(n*maxLen+n-1));
    int k = 0;
    char *ref = *str;
    for (int i=0; i<n; i++){
        strcpy(ref, arr[i]);
        int len = strlen(arr[i]);
        ref[len] = ' ';
        ref+=len+1;
    }
    // printf("String\n");
    // printf("%s\n", *str);
}

int main(int argc, char *argv[]){
    suffixTree st;
    initSuffixTree(&st);
    // int maxLength = maxLen(argc, argv);
    // // printf("maxLength: %d\n", maxLength);
    // // printf("maxLength: %d\n", maxLength);
    // // printf("---\n");
    // char **arr = (char **) malloc(sizeof(char *)*atoi(argv[2]));
    // get_me_random_n_words(argc, argv, maxLength, arr);
    // // displaySelectedWords(arr, atoi(argv[2]));
    // char *temp;
    // makeString(arr, atoi(argv[2]), &temp, maxLength);
    // printf("%s\n", temp);
    // printf("%p %s\n", arr, arr[0]);
    // printf("---\n");
    // printf("Here\n");
    char *str = "hello#world#pop#push#wondering#whirlpool#won";
    // temp = "world";
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
    // checkForSubString(st, "worln");
    didYouMean(st, "#whee");
    // // checkForSubString(st, "SSI");
    // // int count = countOccurences(&st, "AGATC");
    // // printf("count: %d\n", count);
    // // printf("longest repeated substring: %d\n", longestRepeatedSubstring(&st));
    // printf("longest common substring: %d\n", longestCommonSubstring(&st));
    // // printRoot2Leaf(&st);

    // check(st.root->children[35], st.str);
    // printf("\n");
}
