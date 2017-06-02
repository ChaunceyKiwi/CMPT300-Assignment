#include "gtest/gtest.h"
#include "../include/list.h"
#include <stdio.h>

/* unit_test7 focus on testing of ListSearch() */

int intEqualTo(void* item, void* comparisonArg) {
  return *(int*)(item) == *(int*)(comparisonArg);
}

int intLargerThan(void* item, void* comparisonArg) {
  return *(int*)(item) > *(int*)(comparisonArg);
}

int intSmallerThan(void* item, void* comparisonArg) {
  return *(int*)(item) < *(int*)(comparisonArg);
}

TEST(ListSearch, searchFailBecaseOfNotExisting) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  // search from the first item
  ListFirst(list);
  int gg = 5;

  EXPECT_TRUE(ListSearch(list, intEqualTo, &gg) == NULL);
  EXPECT_TRUE(ListCurr(list) == NULL);
  EXPECT_TRUE(list->currFlag == 1);

  // search from the first item
  ListFirst(list);

  EXPECT_TRUE(ListSearch(list, intLargerThan, &gg) == NULL);
  EXPECT_TRUE(ListCurr(list) == NULL);
  EXPECT_TRUE(list->currFlag == 1);
}

TEST(ListSearch, searchFailBecaseOfNotInTheRange) {
  int a = 1, b = 2, c = 8, d = 9;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  int gg = 1;

  // search from the second item
  ListFirst(list);
  ListNext(list);

  EXPECT_TRUE(ListSearch(list, intEqualTo, &gg) == NULL);
  EXPECT_TRUE(ListCurr(list) == NULL);
  EXPECT_TRUE(list->currFlag == 1);

  // search from the third item
  ListFirst(list); // 1
  ListNext(list);  // 2
  ListNext(list);  // 8

  gg = 4;
  EXPECT_TRUE(ListSearch(list, intSmallerThan, &gg) == NULL);
  EXPECT_TRUE(ListCurr(list) == NULL);
  EXPECT_TRUE(list->currFlag == 1);
}

TEST(ListSearch, searchSuccess1) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  // search from the first item
  ListFirst(list);
  int gg = 4;

  EXPECT_TRUE(*(int*)ListSearch(list, intEqualTo, &gg) == 4);

  // search from the first item
  ListFirst(list);

  gg = 2;
  EXPECT_TRUE(*(int*)ListSearch(list, intLargerThan, &gg) == 3);
}

TEST(ListSearch, searchSuccess2) {
  int a = 1, b = 2, c = 8, d = 9;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  int gg = 2;

  // search from the second item
  ListFirst(list);
  ListNext(list);

  EXPECT_TRUE(*(int*)ListSearch(list, intEqualTo, &gg) == 2);

  // search from the second item
  ListFirst(list); // 1
  ListNext(list);  // 2

  gg = 4;
  EXPECT_TRUE(*(int*)ListSearch(list, intSmallerThan, &gg) == 2);
}
