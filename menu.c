#include <stdio.h>
#include <stdlib.h>
#include "suffixTree.h"

int main(){
    suffixTree st;
    initSuffixTree(&st);
    char *str = "xyzxy";
    preprocessString(&st, str);
    printf("%s\n", st.str);
    buildSuffixTree(&st);
    Display(st.root);
    // for (int i=0; i<256; i++){
    //     if (st.root->children[i]){
    //         printf("%c\n", i);
    //         printf("%d %d\n", *(st.root->children[i]->start), *(st.root->children[i]->end));
    //     }
    // }
}
