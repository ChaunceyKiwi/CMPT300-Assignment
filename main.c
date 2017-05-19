#include <stdio.h>
#include "list.h"

int main(void)
{
  LIST *temp = ListCreate();
  int a = 5;
  ListPrepend(temp, &a);
  int b = 6;
  ListPrepend(temp, &b);
  int c = 7;
  ListPrepend(temp, &c);
  int d = 8;
  ListInsert(temp, &d);
  
  printf("%d\n", *(int*)ListLast(temp));
  printf("%d\n", *(int*)ListPrev(temp));
  printf("%d\n", *(int*)ListPrev(temp));
  printf("%d\n", *(int*)ListPrev(temp));
  
  return 0;
}
