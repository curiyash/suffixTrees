#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "dnaSuffixTree.h"
#include "dnaUtilities.h"
#include "lca.h"

void Menu(){
    printf("\nGeneral Menu\n");
    printf("-----------------\n");
    int i = 0;
    printf("%d. Exit\n", i++);
    printf("%d. Load a (new) word from text file\n", i++);
    printf("%d. Check for a substring in word\n", i++);
    printf("%d. Longest Repeated Substring in word\n", i++);
    printf("%d. Longest Normal Palindromic Substring in word\n", i++);
    printf("%d. Inexact Matching\n", i++);
    printf("%d. Longest Common Substring between 2 words\n", i++);
    printf("%d. Count occurrences of a substring in word\n", i++);
    printf("%d. Find longest continuous repeat of given pattern in word\n", i++);
    printf("\n");
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
    long long **spt = NULL;
    suffixTree st;
    initSuffixTree(&st, 256, 0);
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
                initSuffixTree(&pal, 256, 0);
                preprocessStringForLPS(&pal, st.str);
                buildSuffixTree(&pal);
                spt = lca(&pal, spt);
                freeSuffixTree(pal.root, &freeNodes);
                printf("freeNodes: %d\n", freeNodes);
                freeNodes = 0;
                break;
            case 5: // Longest Complement Palindromic Substring
                initSuffixTree(&pal, 256, 0);
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
                        free(spt);
                        spt = NULL;
                    }
                } else {
                    status = LoadPat(&pat);
                    if (status){
                        printf("An error occurred\n");
                        continue;
                    }
                    free(spt);
                    spt = NULL;
                    loadedPat = 1;
                }
                initSuffixTree(&pal, 256, 0);
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
                initSuffixTree(&pal, 256, 0);
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
    printf("Clearing memory. Please wait...\n");
    freeSuffixTree(st.root, &freeNodes);
    free(spt);
    printf("Done!\nHave a nice day\n");
}

// int main(int argc, char *argv[]){
//     suffixTree st;
//     initSuffixTree(&st);
//     // int maxLength = maxLen(argc, argv);
//     // printf("maxLength: %d\n", maxLength);
//     // printf("maxLength: %d\n", maxLength);
//     // printf("---\n");
//     // char **arr = (char **) malloc(sizeof(char *)*atoi(argv[2]));
//     // get_me_random_n_words(argc, argv, maxLength, arr);
//     // displaySelectedWords(arr, atoi(argv[2]));
//     // printf("%p %s\n", arr, arr[0]);
//     // printf("---\n");
//     // printf("Here\n");
//     char *str = "AGATTAGC#CGATTAGA";
//     preprocessString(&st, str);
//     printf("%s\n", st.str);
//     buildSuffixTree(&st);
//     printf("Here\n");
//     // Display(st.root, st.str);
//     // printf("---------------------------------------------------------\n");
//     // // for (int i=0; i<256; i++){
//     // //     if (st.root->children[i]){
//     // //         printf("%c\n", i);
//     // //         printf("%d %d\n", *(st.root->children[i]->start), *(st.root->children[i]->end));
//     // //     }
//     // // }
//     // // checkForSubString(st, "ip");
//     // // checkForSubString(st, "SSI");
//     // // int count = countOccurences(&st, "AGATC");
//     // // printf("count: %d\n", count);
//     // printf("longest repeated substring: %d\n", longestRepeatedSubstring(&st));
//     // printf("longest common substring: %d\n", longestCommonSubstring(&st));
//     // printRoot2Leaf(&st);

//     // check(st.root, st.str);
//     // printf("\n");
// }
