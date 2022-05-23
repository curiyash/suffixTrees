// The maximum number of children a node could have
// 256 ASCII characters
// Could we do something better instead?
#define MAX_CHAR 256

typedef struct node{
    // Has an array representing possibly 256 ASCII characters
    // Let's start with 26 first
    struct node *children[MAX_CHAR];

    // Every node has a start and end index of edge from its parent
    int *start;
    int *end;

    // Every internal node has a suffix link
    struct node *suffixLink;

    // suffixIndex for leaf
    int suffixIndex;
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
    // Includes active points
    activePoint ap;
    // Include count for pending suffixes
    int remaining;
    // Includes global end value
    int end;
} suffixTree;

void initSuffixTree(suffixTree *st);
void preprocessString(suffixTree *st, char *str);
void buildSuffixTree(suffixTree *st);
void Display(node *t, char *str);
int checkForSubString(suffixTree st, char *pat);
