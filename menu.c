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

int main(){
    suffixTree st;
    initSuffixTree(&st);
    char *str = "messiahippi#mississippi";
    preprocessString(&st, str);
    printf("%s\n", st.str);
    buildSuffixTree(&st);
    Display(st.root, st.str);
    printf("---------------------------------------------------------\n");
    // for (int i=0; i<256; i++){
    //     if (st.root->children[i]){
    //         printf("%c\n", i);
    //         printf("%d %d\n", *(st.root->children[i]->start), *(st.root->children[i]->end));
    //     }
    // }
    // checkForSubString(st, "GTTT");
    // checkForSubString(st, "SSI");
    // printf("count: %d\n", countOccurences(&st, "s"));
    // printf("longest repeated substring: %d\n", longestRepeatedSubstring(&st));
    printf("longest common substring: %d\n", longestCommonSubstring(&st));
    // printRoot2Leaf(&st);

    // check(st.root, st.str);
}
