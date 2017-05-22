#include <stdio.h>
#include "list.h"

int main(void)
{
  LIST *list1 = ListCreate();
  LIST *list2 = ListCreate();
  
  // initialization of temp1
  int a = 1;
  ListAppend(list1, &a);
  
  int b = 2;
  ListAppend(list1, &b);
  
  int c = 3;
  ListAppend(list1, &c);
  
  int d = 4;
  ListAppend(list1, &d);
  
  // initialization of temp2
  int e = 5;
  ListAppend(list2, &e);
  
  int f = 6;
  ListAppend(list2, &f);
  
  int g = 7;
  ListAppend(list2, &g);
  
  int h = 8;
  ListAppend(list2, &h);
  
  printf("Before concatenation: \n");
  ListPrint(list1);
  ListPrint(list2);
  
  ListConcat(list1, list2);
  
  printf("After concatenation: \n");
  ListPrint(list1);
  
  return 0;
}
