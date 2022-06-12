#include <stdio.h>
#include <stdlib.h>
#include "dnaSuffixTree.h"
#include "utilities.h"

char Encode(int a){
    switch (a){
        case 0: return '\0';
        case 1: return '#';
        case 2: return '$';
        case 3: return 'A';
        case 4: return 'C';
        case 5: return 'G';
        case 6: return 'T';
        default: return '\0';
    }   
}

void check(node *root, char *str, suffixTree *st){
    for (int i=0; i<7; i++){
        if (root->children[i]){
            printf("parent: %d %d\n", *(root->start), *(root->end));
            // for (int j=*(root->start); j<=*(root->end); j++){
            //     printf("%c", str[j]);
            // }
            // printf("\n");
            printf("%c\n", Encode(i));
            printf("%d %d\n", *(root->children[i]->start), *(root->children[i]->end));
            check(root->children[i], str, st);
        }
    }
}

void WhereIsItGoingWrong(suffixTree *st){
    printf("%d %d\n", *(st->root->start), *(st->root->end));
}

int main(int argc, char *argv[]){
    if (argc==2){
        FILE *fp = fopen(argv[1], "r");
        printf("%s\n", argv[1]);
        if (!fp){
            printf("Error opening file\n");
            return 1;
        }
        char c;
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
        // printf("count: %d\n", count);
        // printf("str: %s\n", str);
        suffixTree st;
        initSuffixTree(&st, 256, 0);
        // printf("%p %s\n", arr, arr[0]);
        // printf("---\n");
        printf("Here\n");
        preprocessString(&st, str);
        printf("%s\n", st.str);
        printf("42\n");
        buildSuffixTree(&st);
        // printf("%d %d\n", *(st.root->start), *(st.root->end));
        // Display(st.root, st.str);
        // printf("---------------------------------------------------------\n");
        // // for (int i=0; i<256; i++){
        // //     if (st.root->children[i]){
        // //         printf("%c\n", i);
        // //         printf("%d %d\n", *(st.root->children[i]->start), *(st.root->children[i]->end));
        // //     }
        // // }
        // checkForSubString(st, "AG");
        // // checkForSubString(st, "SSI");
        // int count = countOccurences(&st, "sss");
        // int count = 0;
        // printf("\n");
        // countRepeats(&st, "A", st.root, &count);
        // printf("count: %d\n", count);
        // printf("%ld\n", sizeof(st.root->children));
        // WhereIsItGoingWrong(&st);
        printf("longest repeated substring: %d\n", longestRepeatedSubstring(&st));
        // printf("longest common substring: %d\n", longestCommonSubstring(&st));
        // // printRoot2Leaf(&st);

        // check(st.root, st.str, &st);
        // printf("\n");
        fclose(fp);
        return 0;
    } else{
        printf("Need 1 argument: <filename>.txt to read the string from\n");
        return 1;
    }
}
