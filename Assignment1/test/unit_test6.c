#include "gtest/gtest.h"
#include "../include/list.h"
#include <stdio.h>

/* unit_test6 focus on testing of ListRemove() and ListTrim() */

TEST(ListRemoveTrim, removeEmptyList) {
  LIST* list = ListCreate();
  EXPECT_TRUE(ListRemove(list) == NULL);
  EXPECT_TRUE(ListCurr(list) == NULL);
}

TEST(ListRemoveTrim, trimEmptyList) {
  LIST* list = ListCreate();
  EXPECT_TRUE(ListTrim(list) == NULL);
  EXPECT_TRUE(ListCurr(list) == NULL);
}

TEST(ListRemoveTrim, removeBeyondTail) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  ListLast(list);
  EXPECT_TRUE(list->currFlag == 0);
  ListNext(list);
  EXPECT_TRUE(list->currFlag == 1);

  EXPECT_TRUE(ListRemove(list) == NULL);
}

TEST(ListRemoveTrim, trimBeyondTail) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  ListLast(list);
  EXPECT_TRUE(list->currFlag == 0);
  ListNext(list);
  EXPECT_TRUE(list->currFlag == 1);

  EXPECT_TRUE(*(int*)ListTrim(list) == 4);
}

TEST(ListRemoveTrim, removeBeforeHead) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  ListFirst(list);
  EXPECT_TRUE(ListCurr(list) != NULL);
  EXPECT_TRUE(list->currFlag == 0);
  ListPrev(list);
  EXPECT_TRUE(ListCurr(list) == NULL);
  EXPECT_TRUE(list->currFlag == -1);

  EXPECT_TRUE(ListRemove(list) == NULL);
}

TEST(ListRemoveTrim, trimBeforeHead) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  ListFirst(list);
  EXPECT_TRUE(ListCurr(list) != NULL);
  EXPECT_TRUE(list->currFlag == 0);
  ListPrev(list);
  EXPECT_TRUE(ListCurr(list) == NULL);
  EXPECT_TRUE(list->currFlag == -1);

  EXPECT_TRUE(*(int*)ListTrim(list) == 4);
}

TEST(ListRemoveTrim, removeTheLastItem) {
  int a = 1;
  LIST* list = ListCreate();
  ListAppend(list, &a);

  EXPECT_TRUE(*(int*)ListRemove(list) == 1);
  EXPECT_TRUE(ListFirst(list) == NULL);
  EXPECT_TRUE(ListLast(list) == NULL);
  EXPECT_TRUE(ListCurr(list) == NULL);
  EXPECT_TRUE(list->currFlag = -1);
  EXPECT_TRUE(ListCount(list) == 0);
}

TEST(ListRemoveTrim, removeTheHead) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);
  ListFirst(list);

  EXPECT_TRUE(ListCount(list) == 4);

  EXPECT_TRUE(*(int*)ListRemove(list) == 1);
  EXPECT_TRUE(*(int*)ListFirst(list) == 2);
  EXPECT_TRUE(*(int*)ListNext(list) == 3);
  EXPECT_TRUE(*(int*)ListNext(list) == 4);

  EXPECT_TRUE(ListCount(list) == 3);
}

TEST(ListRemoveTrim, removeTheTail) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);
  ListLast(list);

  EXPECT_TRUE(ListCount(list) == 4);

  EXPECT_TRUE(*(int*)ListRemove(list) == 4);
  EXPECT_TRUE(*(int*)ListFirst(list) == 1);
  EXPECT_TRUE(*(int*)ListNext(list) == 2);
  EXPECT_TRUE(*(int*)ListNext(list) == 3);

  EXPECT_TRUE(ListCount(list) == 3);
}

TEST(ListRemoveTrim, removeMiddle) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);
  ListLast(list);
  ListPrev(list);

  EXPECT_TRUE(ListCount(list) == 4);

  EXPECT_TRUE(*(int*)ListRemove(list) == 3);
  EXPECT_TRUE(*(int*)ListFirst(list) == 1);
  EXPECT_TRUE(*(int*)ListNext(list) == 2);
  EXPECT_TRUE(*(int*)ListNext(list) == 4);

  EXPECT_TRUE(ListCount(list) == 3);
}
