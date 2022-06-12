#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "suffixTree.h"
#include "utilities.h"
#include "lca.h"

void Menu(){
    printf("\nDNA Sequence Menu\n");
    printf("-----------------\n");
    int i = 0;
    printf("%d. Exit\n", i++);
    printf("%d. Load a (new) sequence from text file\n", i++);
    printf("%d. Check for a substring in sequence\n", i++);
    printf("%d. Longest Repeated Substring in sequence\n", i++);
    printf("%d. Longest Normal Palindromic Substring in sequence\n", i++);
    printf("%d. Longest Complement Palindrome Substring in sequence\n", i++);
    printf("%d. Approximate Sequence Matching\n", i++);
    printf("%d. Longest Common Substring between 2 sequences\n", i++);
    printf("%d. Count occurrences of a substring in sequence\n", i++);
    printf("%d. Find longest continuous occurrence of given STR in sequence\n", i++);
    printf("\n\n");
}

int main(){
    // Helper variables
    int loaded = 0;
    int loadedPat = 0;
    char *pat;
    int goAhead = 0;
    int op_num;
    int k = 0;
    int freeNodes = 0;
    int status = 0;
    long long **spt;
    suffixTree st;
    initSuffixTree(&st, 7, 1);
    suffixTree pal;
    int loop = 1;
    int patLen = 0;

    // Main loop
    while (loop){
        Menu();
        printf("Enter option number: ");
        scanf("%d", &op_num);
        fflush(stdin);
        printf("\n");
        switch(op_num){
            case 0: loop = 0;
                break;
            case 1: // load from file
                loaded = Load(&st, loaded);
                if (!loaded){
                    free(spt);
                    spt = NULL;
                }
                continue;
            case 2: // checkForSubstring
                printf("Load pattern from a textfile or input in the command line\n");
                if (loadedPat){
                    printf("Pattern is already loaded - %s. Use the same pattern? 1/0\n", pat);
                    scanf("%d", &goAhead);
                    if (!goAhead){
                        int status = LoadPat(&pat);
                        if (status){
                            printf("An error occurred\n");
                            continue;
                        }
                    }
                }
                int status = LoadPat(&pat);
                if (status){
                    printf("An error occurred\n");
                    continue;
                }
                // Return positions of all occurrences
                checkForSubString(&st, pat);
                break;
            case 3: // Longest Repeated Substring
                printf("Length: %d\n", longestRepeatedSubstring(&st));
                break;
            case 4: // Longest Normal Palindromic Substring
                // Make new suffixTree
                initSuffixTree(&pal, 7, 1);
                preprocessStringForLPS(&pal, st.str);
                buildSuffixTree(&pal);
                spt = lca(&pal, spt);
                freeSuffixTree(pal.root, &freeNodes);
                printf("freeNodes: %d\n", freeNodes);
                freeNodes = 0;
                break;
            case 5: // Longest Complement Palindromic Substring
                initSuffixTree(&pal, 7, 1);
                makeComplementReverse(&pal, st.str);
                buildSuffixTree(&pal);
                spt = lca(&pal, spt);
                printf("freeNodes: %d\n", freeNodes);
                freeNodes = 0;
                break;
            case 6: // Approximate Sequence Matching
                printf("Load pattern from a textfile or input in the command line\n");
                if (loadedPat){
                    printf("Pattern is already loaded - %s. Use the same pattern? 1/0\n", pat);
                    scanf("%d", &goAhead);
                    if (!goAhead){
                        status = LoadPat(&pat);
                        if (status){
                            printf("An error occurred\n");
                            continue;
                        }
                    }
                    loadedPat = 1;
                    free(spt);
                }
                initSuffixTree(&pal, 7, 1);
                status = LoadPat(&pat);
                if (status){
                    printf("An error occurred\n");
                    loadedPat = 0;
                    free(spt);
                    continue;
                }
                loadedPat = 1;
                preprocessStringForKMisMatch(&pal, st.str, pat);
                printf("%s\n", pal.str);
                buildSuffixTree(&pal);
                // lca(&pal);
                patLen = strlen(pat);
                printf("patLen: %d\n", patLen);
                printf("Enter value of k (number of mismatches allowed): ");
                scanf("%d", &k);
                if (!k){
                    printf("Can't be 0\n");
                    continue;
                }
                spt = kMisMatch(&pal, k, patLen, spt);
                freeSuffixTree(pal.root, &freeNodes);
                break;
            case 7: // Longest Common Substring
                initSuffixTree(&pal, 7, 1);
                status = LoadPat(&pat);
                if (status){
                    printf("An error occurred\n");
                    continue;
                }
                preprocessStringForLCS(&pal, st.str, pat);
                printf("%s\n", pal.str);
                buildSuffixTree(&pal);
                longestCommonSubstring(&pal);
                freeSuffixTree(pal.root, &freeNodes);
                break;
            case 8: // Count occurrences
                status = LoadPat(&pat);
                if (status){
                    printf("An error occurred\n");
                    continue;
                }
                printf("Count: %d\n", countOccurences(&st, pat));
                break;
            case 9: // Count occurrence of given STR
                status = LoadPat(&pat);
                if (status){
                    printf("An error occurred\n");
                    continue;
                }
                int count = 0;
                countRepeats(&st, pat, st.root, &count);
                printf("Longest Continuous Repeat: %d\n", count);
                break;
            default: printf("Invalid option\n"); break;
        }
    }
    // if (argc==2){
    //     FILE *fp = fopen(argv[1], "r");
    //     printf("%s\n", argv[1]);
    //     if (!fp){
    //         printf("Error opening file\n");
    //         return 1;
    //     }
    //     // printf("count: %d\n", count);
    //     // printf("str: %s\n", str);
    //     suffixTree st;
    //     initSuffixTree(&st, 256, 0);
    //     // printf("%p %s\n", arr, arr[0]);
    //     // printf("---\n");
    //     printf("Here\n");
    //     preprocessString(&st, str);
    //     printf("%s\n", st.str);
    //     printf("42\n");
    //     buildSuffixTree(&st);
    //     // printf("%d %d\n", *(st.root->start), *(st.root->end));
    //     // Display(st.root, st.str);
    //     // printf("---------------------------------------------------------\n");
    //     // // for (int i=0; i<256; i++){
    //     // //     if (st.root->children[i]){
    //     // //         printf("%c\n", i);
    //     // //         printf("%d %d\n", *(st.root->children[i]->start), *(st.root->children[i]->end));
    //     // //     }
    //     // // }
    //     // checkForSubString(st, "AG");
    //     // // checkForSubString(st, "SSI");
    //     // int count = countOccurences(&st, "sss");
    //     // int count = 0;
    //     // printf("\n");
    //     // countRepeats(&st, "A", st.root, &count);
    //     // printf("count: %d\n", count);
    //     // printf("%ld\n", sizeof(st.root->children));
    //     // WhereIsItGoingWrong(&st);
    //     printf("longest repeated substring: %d\n", longestRepeatedSubstring(&st));
    //     // printf("longest common substring: %d\n", longestCommonSubstring(&st));
    //     // // printRoot2Leaf(&st);
    //     // check(st.root, st.str, &st);
    //     // printf("\n");
    //     fclose(fp);
    //     return 0;
    // } else{
    //     printf("Need 1 argument: <filename>.txt to read the string from\n");
    //     return 1;
    // }
    printf("Clearing memory. Please wait...\n");
    freeSuffixTree(st.root, &freeNodes);
    free(spt);
    printf("Done!\nHave a nice day\n");
}
