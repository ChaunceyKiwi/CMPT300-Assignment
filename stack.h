#ifndef stack_h
#define stack_h

#include <stdio.h>

#define stackMaxSize 1000

typedef struct stack_ {
  int values[1000];
  int topIndex;
} STACK;

int isEmpty(STACK* stack);
int top(STACK* stack);
int pop(STACK* stack);
int push(STACK* stack, int val);

#endif /* stack_h */
