typedef struct elem{
    struct elem *next;
    int start;
}elem;

typedef struct{
    int maxLen;
    elem *e;
} stack;

void initStack(stack *s);
void push(stack *s, int start);
void pop(stack *s);
elem *top(stack *s);
int isEmpty(stack *s);
