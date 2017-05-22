#include <stdio.h>
#include "list.h"

// test on the pipeline: ListCreate(), ListNext()

int main(void)
{
  LIST *temp = ListCreate();
  
  // since current pointer is before the head, should return NULL
  if (ListNext(temp) == NULL) {
    printf("Test Passed!\n");
  }
  
  return 0;
}
