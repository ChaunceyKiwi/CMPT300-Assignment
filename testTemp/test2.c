#include <stdio.h>
#include "list.h"
#include "stack.h"

int main(void)
{
  STACK stack;
  stack.topIndex = -1;
  STACK* stackPtr = &stack;

  push(stackPtr, 1);
  push(stackPtr, 2);
  push(stackPtr, 3);
  push(stackPtr, 4);
  push(stackPtr, 5);
  printf("%d ", pop(stackPtr));
  printf("%d ", pop(stackPtr));
  printf("%d ", pop(stackPtr));
  printf("%d ", pop(stackPtr));
  printf("%d\n", pop(stackPtr));
  
  return 0;
}
