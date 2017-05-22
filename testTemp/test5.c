#include <stdio.h>
#include "list.h"

// test on the pipeline: ListCreate(), ListAdd()

int main(void)
{
  LIST *temp = ListCreate();
  
  int a = 5;
  ListAdd(temp, &a);
  
  
  printf("%d\n", *(int*)ListFirst(temp));
  
  return 0;
}
