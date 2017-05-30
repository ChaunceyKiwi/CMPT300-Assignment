#include "gtest/gtest.h"
#include "../include/list.h"
#include <stdio.h>

/* unit_test5 focus on testing of ListAppend() and ListPrepend() */

TEST(ListAppendPrepend, appendEmptyList) {
  int a = 1;
  LIST* list = ListCreate();

  EXPECT_TRUE(list->currFlag == -1);
  EXPECT_TRUE(ListCount(list) == 0);
  ListAppend(list, &a);
  EXPECT_TRUE(1 == *(int*)ListFirst(list));
  EXPECT_TRUE(1 == *(int*)ListLast(list));
  EXPECT_TRUE(ListCount(list) == 1);
  EXPECT_TRUE(list->currFlag == 0);
}

TEST(ListAppendPrepend, prependEmptyList) {
  int a = 1;
  LIST* list = ListCreate();

  EXPECT_TRUE(list->currFlag == -1);
  EXPECT_TRUE(ListCount(list) == 0);
  ListPrepend(list, &a);
  EXPECT_TRUE(1 == *(int*)ListFirst(list));
  EXPECT_TRUE(1 == *(int*)ListLast(list));
  EXPECT_TRUE(ListCount(list) == 1);
  EXPECT_TRUE(list->currFlag == 0);
}

TEST(ListAppendPrepend, appendNormalList) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);

  ListAppend(list, &d);

  EXPECT_TRUE(*(int*)ListFirst(list) == 1);
  EXPECT_TRUE(*(int*)ListNext(list) == 2);
  EXPECT_TRUE(*(int*)ListNext(list) == 3);
  EXPECT_TRUE(*(int*)ListNext(list) == 4);
}

TEST(ListAppendPrepend, prependNormalList) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);

  ListPrepend(list, &d);

  EXPECT_TRUE(*(int*)ListFirst(list) == 4);
  EXPECT_TRUE(*(int*)ListNext(list) == 1);
  EXPECT_TRUE(*(int*)ListNext(list) == 2);
  EXPECT_TRUE(*(int*)ListNext(list) == 3);
}