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

  printf("Count: %d\n", ListCount(list1));
  ListPrint(list1);
  printf("\n");
  
  ListTrim(list1);
  printf("Count: %d\n", ListCount(list1));
  ListPrint(list1);
  printf("\n");
  
  ListTrim(list1);
  printf("Count: %d\n", ListCount(list1));
  ListPrint(list1);
  printf("\n");
  
  ListTrim(list1);
  printf("Count: %d\n", ListCount(list1));
  ListPrint(list1);
  printf("\n");
  
  ListTrim(list1);
  printf("Count: %d\n", ListCount(list1));
  ListPrint(list1);
  printf("\n");
  
  return 0;
}
