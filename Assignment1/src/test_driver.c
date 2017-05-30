#include <stdio.h>
#include <stdlib.h>
#include "../include/list.h"

/***********************************************************
*   User defined functions declaration
*/

int intEqualTo(LIST* list, void* comparisonArg);
int intLargerThan(LIST* list, void* comparisonArg);
void freeItem(void* item);

int main(void)
{
  /* Initialization, using ListCreate(), ListAppend() and ListPrepend() */
  LIST *list1 = ListCreate();
  LIST *list2 = ListCreate();

  int *a = (int*)malloc(sizeof(int));
  int *b = (int*)malloc(sizeof(int));
  int *c = (int*)malloc(sizeof(int));
  int *d = (int*)malloc(sizeof(int));
  int *e = (int*)malloc(sizeof(int));
  int *f = (int*)malloc(sizeof(int));
  int *g = (int*)malloc(sizeof(int));

  *a = 1;
  *b = 2;
  *c = 3;
  *d = 4;
  *e = 10;
  *f = 20;
  *g = 30;

  ListAppend(list1, a);
  ListAppend(list1, b);
  ListAppend(list1, c);
  ListAppend(list1, d);
  ListPrepend(list2, e);
  ListPrepend(list2, f);
  ListPrepend(list2, g);

  /* Testing on ListCreate(), ListAppend() and ListPrepend() */
  printf("1. Initialization with ListCreate(), ListAppend() and ListPrepend()\n");
  printf("  List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("  List2 with length %d: ", ListCount(list2));
  ListPrint(list2);
  printf("(The length of the list is given by ListCount())\n\n");

  /* Testing on ListInsert() and ListAdd() */
  printf("2. Testing on perform of ListAdd() and ListInsert() on head and tail,\n");
  ListFirst(list1);
  ListLast(list2);
  int *h = (int*)malloc(sizeof(int));
  int *i = (int*)malloc(sizeof(int));
  *h = 5;
  *i = 40;
  ListAdd(list1, h);
  ListInsert(list2, i);
  printf("  After ListAdd() on List1 at head:\n");
  printf("    List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("  After ListInsert() on List2 at tail:\n");
  printf("    List2 with length %d: ", ListCount(list2));
  ListPrint(list2);

  /* Testing on ListFirst(), ListLast(), ListPrev(), ListNext() */
  printf("\n3. Testing on ListFirst(), ListLast(), ListPrev(), ListNext()\n");
  printf("  First and second items of list1: %d %d\n", *(int*)ListFirst(list1), *(int*)ListNext(list1));
  printf("  Last and Next-to-last items of list2: %d %d\n\n", *(int*)ListLast(list2), *(int*)ListPrev(list2));

  /* Testing on ListCurr(), ListRemove() and ListTrim() */
  printf("4. Testing on ListCurr(), ListRemove() and ListTrim()\n");
  printf("  Current items of list1 and list2: %d %d\n", *(int*)ListCurr(list1), *(int*)ListCurr(list2));
  printf("  Before ListRemove():\n");
  printf("    List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    List2 with length %d: ", ListCount(list2));
  ListPrint(list2);
  ListRemove(list1);
  ListRemove(list2);
  printf("  After ListRemove():\n");
  printf("    List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    List2 with length %d: ", ListCount(list2));
  ListPrint(list2);
  printf("  Before ListTrim():\n");
  printf("    List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    List2 with length %d: ", ListCount(list2));
  ListPrint(list2);
  ListTrim(list1);
  ListTrim(list2);
  printf("  After ListTrim():\n");
  printf("    List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    List2 with length %d: ", ListCount(list2));
  ListPrint(list2);

  /* Testing on ListConcat() */
  printf("\n5. Testing on ListConcat()\n");
  printf("  Before ListConcat():\n");
  printf("    List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    List2 with length %d: ", ListCount(list2));
  ListPrint(list2);
  ListConcat(list1, list2);
  printf("  After ListConcat():\n");
  printf("    List1 with length %d: ", ListCount(list1));
  ListPrint(list1);

  /* Testing on ListSearch() */
  printf("\n6. Testing on ListSearch()\n");
  printf("  List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("  Try to find the first item larger than 10 in list1\n");
  int ten = 10;
  int *result1 = (int*)ListSearch(list1, intLargerThan, &ten);
  if (result1 == NULL) {
    printf("    Searching result: Not found\n");
  } else {
    printf("    Searching result: The item is %d \n", *result1);
  }
  printf("  Try to find the first item larger than 10 in list1\n");
  int fifty = 50;
  int *result2 = (int*)ListSearch(list1, intLargerThan, &fifty);
  if (result2 == NULL) {
    printf("    Searching result: Not found\n");
  } else {
    printf("    Searching result: The item is %d \n", *result2);
  }

  /* Testing on ListFree() */
  ListFree(list1, freeItem);

  return 0;
}

/***********************************************************
*   User defined functions implementation
*/

int intEqualTo(LIST* list, void* comparisonArg) {
  return *(int*)(list->curr->val) == *(int*)(comparisonArg);
}

int intLargerThan(LIST* list, void* comparisonArg) {
  return *(int*)(list->curr->val) > *(int*)(comparisonArg);
}

void freeItem(void* item) {
  if (item != NULL) {
    free(item);
  }
}

