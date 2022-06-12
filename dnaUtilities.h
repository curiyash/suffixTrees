// Includes utility functions on suffix tree
extern int MAX_CHAR;

typedef struct pair{
    int begin;
    int end;
    struct pair *next;
} pair;

typedef struct together{
    int lcs;
    struct pair *next;
} together;

typedef struct matchPos{
    int length;
    struct pair *next;
} matchPos;

int checkForSubString(suffixTree *st, char *pat);
int countOccurences(suffixTree *st, char *pat);
int longestRepeatedSubstring(suffixTree *st);
int longestCommonSubstring(suffixTree *st);
int countRepeats(suffixTree *st, char *pat, node *curr, int *count);
void preprocessStringForLCS(suffixTree *st, char *str, char *pat);
// int countRepeats(suffixTree *st, char *pat);