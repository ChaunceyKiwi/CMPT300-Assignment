#include "stack.h"

int isEmpty(STACK* stack) {
  if (stack->topIndex == -1) {
    return 1;
  } else {
    return 0;
  }
}

int isFull(STACK* stack) {
  if (stack->topIndex >= stackMaxSize - 1) {
    return 1;
  } else {
    return 0;
  }
}

int top(STACK* stack) {
  return stack->values[stack->topIndex];
}

int pop(STACK* stack) {
  if (isEmpty(stack)) {
    return -1;
  } else {
    return stack->values[stack->topIndex--];
  }
}

int push(STACK* stack, int val) {
  if (isFull(stack)) {
    return -1;
  } else {
    stack->values[++stack->topIndex] = val;
    return 0;
  }
}
