#include <stdio.h>
#include "list.h"
#include "stack.h"

// to run test3, set the headsPoolSize as 0

int main(void)
{
  LIST *temp = ListCreate();

  if (temp == NULL) {
    printf("Test Passed!\n");
  }
  
  return 0;
}
