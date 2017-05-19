#include <stdio.h>
#include "list.h"
#include "stack.h"

int main(void)
{
  LIST *temp = ListCreate();
  LIST *temp2 = ListCreate();
  

  int a = 5;
  ListPrepend(temp, &a);
  
  int b = 6;
  ListPrepend(temp, &b);
  ListPrepend(temp2, &b);

  
  int c = 7;
  ListPrepend(temp, &c);
  ListPrepend(temp2, &c);

  
  int d = 8;
  ListInsert(temp, &d);
  ListInsert(temp2, &d);
  
  int e = 9;
  ListPrepend(temp2, &e);
  

  printf("%d ", *(int*)ListLast(temp));
  printf("%d ", *(int*)ListPrev(temp));
  printf("%d ", *(int*)ListPrev(temp));
  printf("%d\n", *(int*)ListPrev(temp));
  
  printf("%d ", *(int*)ListLast(temp2));
  printf("%d ", *(int*)ListPrev(temp2));
  printf("%d ", *(int*)ListPrev(temp2));
  printf("%d\n", *(int*)ListPrev(temp2));
  
  return 0;
}
