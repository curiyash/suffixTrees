// Includes utility functions on suffix tree

typedef struct pair{
    int begin;
    int end;
    struct pair *next;
} pair;

typedef struct together{
    int lcs;
    struct pair *next;
} together;

int checkForSubString(suffixTree st, char *pat);
int countOccurences(suffixTree *st, char *pat);
int longestRepeatedSubstring(suffixTree *st);
int longestCommonSubstring(suffixTree *st);
int countRepeats(suffixTree *st, char *pat, node *curr, int *count);
// int countRepeats(suffixTree *st, char *pat);
