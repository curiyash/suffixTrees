#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void initStack(stack *s){
    s->e = NULL;
    s->maxLen = 0;
}

void push(stack *s, int start){
    elem *new = (elem *) malloc(sizeof(elem));
    if (!new){
        printf("Stack overflow!\n");
        return;
    }
    new->start = start;
    new->next = s->e;
    s->e = new;
}

void pop(stack *s){
    if (s->e){
        elem *ref = s->e;
        s->e = ref->next;
        ref->next = NULL;
        free(ref);
    }
}

elem *top(stack *s){
    return s->e;
}

int isEmpty(stack *s){
    if (!(s->e)){
        return 1;
    } else{
        return 0;
    }
}
