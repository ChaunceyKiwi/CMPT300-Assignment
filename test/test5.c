#include <stdio.h>
#include "list.h"

// test on the pipeline: ListCreate(), ListAdd()

int main(void)
{
  LIST *temp = ListCreate();
  
  int a = 5;
  ListAdd(temp, &a);
  
//  ListFirst(temp); // current pointer at head 5
//  ListNext(temp);  // current pointer at 6
//  ListNext(temp);  // current pointer at 7
//  ListNext(temp);  // current pointer at tail 8
//  ListNext(temp);  // current pointer beyond tail
  
  
  printf("%d\n", *(int*)ListFirst(temp));
//  printf("%d ", *(int*)ListNext(temp));
//  printf("%d ", *(int*)ListNext(temp));
//  printf("%d ", *(int*)ListNext(temp));
//  printf("%d\n", *(int*)ListNext(temp));
  
  return 0;
}
