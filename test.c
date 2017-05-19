#include <stdio.h>
#include "list.h"
#include "stack.h"

int main(void)
{
  LIST *temp = ListCreate();
  
  printf("%d ", ListCount(temp));

  int a = 5;
  ListPrepend(temp, &a);
  printf("%d ", ListCount(temp));
  
  int b = 6;
  ListPrepend(temp, &b);
  printf("%d ", ListCount(temp));

  int c = 7;
  ListPrepend(temp, &c);
  printf("%d ", ListCount(temp));

  int d = 8;
  ListInsert(temp, &d);
  printf("%d\n", ListCount(temp));
  
  printf("%d ", *(int*)ListLast(temp));
  printf("%d ", *(int*)ListPrev(temp));
  printf("%d ", *(int*)ListPrev(temp));
  printf("%d\n", *(int*)ListPrev(temp));
  
  
//  STACK stack;
//  stack.topIndex = -1;
//  STACK* stackPtr = &stack;
//  
//  push(stackPtr, 1);
//  push(stackPtr, 2);
//  push(stackPtr, 3);
//  push(stackPtr, 4);
//  push(stackPtr, 5);
//  printf("%d ", pop(stackPtr));
//  printf("%d ", pop(stackPtr));
//  printf("%d ", pop(stackPtr));
//  printf("%d ", pop(stackPtr));
//  printf("%d\n", pop(stackPtr));
  
  return 0;
}
