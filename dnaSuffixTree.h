// The maximum number of children a node could have
// 256 ASCII characters
// Could we do something better instead?
// #define MAX_CHAR 256

typedef struct node{
    // Has an array representing possibly 256 ASCII characters
    struct node **children;

    // Every node has a start and end index of edge from its parent
    int *start;
    int *end;

    // Every internal node has a suffix link
    struct node *suffixLink;

    // suffixIndex for leaf
    int suffixIndex;

    // for common substring
    int candidate;
} node;

typedef struct activePoint{
    node *activeNode;
    int activeEdge;
    int activeLength;
} activePoint;

typedef struct suffixTree{
    // Contains the root
    node *root;
    // Contains the string which it has created
    char *str;
    // Includes active point information
    activePoint ap;
    // Include count for pending suffixes
    int remaining;
    // Includes global end value
    int end;
} suffixTree;

// Serialization and Deserialization of suffix trees

void initSuffixTree(suffixTree *st, int maxChar, int status);
int freeSuffixTree(node *n, int *count);
int Load(suffixTree *st, int loaded);
int LoadPat(char **pat);
void preprocessString(suffixTree *st, char *str);
int buildSuffixTree(suffixTree *st);
void Display(node *t, char *str);
// int checkForSubString(suffixTree st, char *pat);
int isLeaf(node *n);
void printRoot2Leaf(suffixTree *st);
