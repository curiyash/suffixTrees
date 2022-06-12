typedef struct lcaElem{
    node *n;
    int visited;
    int depth;
} lcaElem;

long long **lca(suffixTree *st, long long **spt);
void preprocessStringForLPS(suffixTree *st, char *str);
void makeComplementReverse(suffixTree *st, char *str);
long long **kMisMatch(suffixTree *st, int k, int patLen, long long **spt);
void preprocessStringForKMisMatch(suffixTree *st, char *str, char *pat);
