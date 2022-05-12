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
}
