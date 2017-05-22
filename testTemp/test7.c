#include <stdio.h>
#include "list.h"

// test on the ListRemove()

int main(void)
{
  LIST *temp = ListCreate();
  
  int a = 5;
  ListAppend(temp, &a);
  
  int b = 6;
  ListAppend(temp, &b);
  
  int c = 7;
  ListAppend(temp, &c);
  
  int d = 8;
  ListAppend(temp, &d);
  
  ListFirst(temp); // current pointer at head 5
//  ListNext(temp);  // current pointer at 6
//  ListNext(temp);  // current pointer at 7
//  ListNext(temp);  // current pointer at tail 8
//  ListNext(temp);  // current pointer beyond tail
  
  ListRemove(temp);
//  printf("%d\n\n", *(int*)ListRemove(temp));
  
  printf("%d\n", *(int*)ListFirst(temp));
  printf("%d\n", *(int*)ListNext(temp));
  printf("%d\n", *(int*)ListNext(temp));
//  printf("%d\n", *(int*)ListNext(temp));
  
  return 0;
}
