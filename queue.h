typedef struct qElem {
    node *n;
    struct qElem *next;
} qElem;

typedef struct queue {
    qElem *front;
    qElem *rear;
} queue;

void initQueue(queue *q);
void enqueue(queue *q, node *n);
void dequeue(queue *q);
node *front(queue q);
int isEmptyQ(queue q);
