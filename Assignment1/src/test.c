/*------------------------------------------------------
 *
 *  test.c
 *
 *  This file contains the testdriver to test all functions
 *  implemented in list.c
 *
 *  Name         : Chauncey Liu
 *  Student ID   : 301295771
 *  SFU username : cla284
 *  Course       : CMPT 300 Operating Systems I, Summer 2017
 *  Instructor   : Harinder Khangura
 *  TA           : Amineh Dadsetan
 *
 *  Created by Chauncey on 2017-05-22.
 *  Copyright (c) 2017 Chauncey. All rights reserved.
 *
 *------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/list.h"

/***********************************************************
*   User defined functions declaration
*/

int intEqualTo(void* item, void* comparisonArg);
int intLargerThan(void* item, void* comparisonArg);
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

  printf("----------------------------------------------------------------------\n");
  printf("|      Format of tests: [Expected output] vs                         |\n");
  printf("|                        Actual output                               |\n");
  printf("----------------------------------------------------------------------\n\n");

  /* Testing on ListCreate(), ListAppend() and ListPrepend() */
  printf("1. Initialization with ListCreate(), ListAppend() and ListPrepend()\n");
  printf("  [List1 with length 4: 1 2 3 4] vs \n");
  printf("   List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("  [List2 with length 3: 30 20 10] vs \n");
  printf("   List2 with length %d: ", ListCount(list2));
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
  printf("    [List1 with length 5: 1 5 2 3 4] vs \n");
  printf("     List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("  After ListInsert() on List2 at tail:\n");
  printf("    [List2 with length 4: 30 20 40 10] vs \n");
  printf("     List2 with length %d: ", ListCount(list2));
  ListPrint(list2);

  /* Testing on ListFirst(), ListLast(), ListPrev(), ListNext() */
  printf("\n3. Testing on ListFirst(), ListLast(), ListPrev(), ListNext()\n");
  printf("  [First and second items of list1: 1 5] vs \n");
  printf("   First and second items of list1: %d ", *(int*)ListFirst(list1));
  printf("%d\n", *(int*)ListNext(list1));
  printf("  [Last and Next-to-last items of list2: 10 40] vs \n");
  printf("   Last and Next-to-last items of list2: %d ", *(int*)ListLast(list2));
  printf("%d\n\n", *(int*)ListPrev(list2));

  /* Testing on ListCurr(), ListRemove() and ListTrim() */
  printf("4. Testing on ListCurr(), ListRemove() and ListTrim()\n");
  printf("  Current items of list1 and list2: 5 40 \n");
  printf("  Current items of list1 and list2: %d ", *(int*)ListCurr(list1));
  printf("%d\n", *(int*)ListCurr(list2));
  printf("  Before ListRemove():\n");
  printf("    [List1 with length 5: 1 5 2 3 4] vs \n");
  printf("     List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    [List2 with length 4: 30 20 40 10] vs \n");
  printf("     List2 with length %d: ", ListCount(list2));
  ListPrint(list2);
  ListRemove(list1);
  ListRemove(list2);
  printf("  After ListRemove():\n");
  printf("    [List1 with length 4: 1 2 3 4] vs \n");
  printf("     List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    [List2 with length 3: 30 20 10] vs \n");
  printf("     List2 with length %d: ", ListCount(list2));
  ListPrint(list2);
  printf("  Before ListTrim():\n");
  printf("    [List1 with length 4: 1 2 3 4] vs \n");
  printf("     List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    [List2 with length 3: 30 20 10] vs \n");
  printf("     List2 with length %d: ", ListCount(list2));
  ListPrint(list2);
  ListTrim(list1);
  ListTrim(list2);
  printf("  After ListTrim():\n");
  printf("    [List1 with length 3: 1 2 3] vs \n");
  printf("     List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    [List2 with length 2: 30 20] vs \n");
  printf("     List2 with length %d: ", ListCount(list2));
  ListPrint(list2);

  /* Testing on ListConcat() */
  printf("\n5. Testing on ListConcat()\n");
  printf("  Before ListConcat():\n");
  printf("    [List1 with length 3: 1 2 3] vs \n");
  printf("     List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    [List2 with length 2: 30 20] vs \n");
  printf("     List2 with length %d: ", ListCount(list2));
  ListPrint(list2);
  ListConcat(list1, list2);
  printf("  After ListConcat():\n");
  printf("    [List1 with length 5: 1 2 3 30 20] vs \n");
  printf("     List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("    [List2 with length 0] vs \n");
  printf("     List2 with length %d\n", ListCount(list2));

  /* Testing on ListSearch() */
  printf("\n6. Testing on ListSearch()\n");
  printf("  List1 with length %d: ", ListCount(list1));
  ListPrint(list1);
  printf("  Try to find the first item larger than 10 in list1\n");
  int ten = 10;
  int *result1 = (int*)ListSearch(list1, intLargerThan, &ten);
  printf("    [Searching result: The item is 30] \n");
  if (result1 == NULL) {
    printf("     Searching result: Not found\n");
  } else {
    printf("     Searching result: The item is %d \n", *result1);
  }
  printf("  Try to find the first item larger than 50 in list1\n");
  int fifty = 50;
  printf("    [Searching result: Not found]\n");
  int *result2 = (int*)ListSearch(list1, intLargerThan, &fifty);
  if (result2 == NULL) {
    printf("     Searching result: Not found\n");
  } else {
    printf("     Searching result: The item is %d \n", *result2);
  }

  /* Testing on ListFree() */
  ListFree(list1, freeItem);

  printf("\n\n-------------------------------Test Finished----------------------------------\n\n");

  return 0;
}

/***********************************************************
*   User defined functions implementation
*/

int intEqualTo(void* item, void* comparisonArg) {
  return *(int*)(item) == *(int*)(comparisonArg);
}

int intLargerThan(void* item, void* comparisonArg) {
  return *(int*)(item) > *(int*)(comparisonArg);
}

void freeItem(void* item) {
  // *(int*)item = 0;
  if (item != NULL) {
    free(item);
  }
}
