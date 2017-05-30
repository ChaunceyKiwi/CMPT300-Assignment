#include "gtest/gtest.h"
#include "../include/list.h"
#include <stdio.h>

/* unit_test4 focus on testing of ListAdd() and ListInsert() */

TEST(ListAddInsert, addEmptyList) {
  int a = 1;
  LIST* list = ListCreate();

  EXPECT_TRUE(list->currFlag == -1);
  EXPECT_TRUE(ListCount(list) == 0);
  ListAdd(list, &a);
  EXPECT_TRUE(1 == *(int*)ListFirst(list));
}

TEST(ListAddInsert, insertEmptyList) {
  int a = 1;
  LIST* list = ListCreate();

  EXPECT_TRUE(list->currFlag == -1);
  EXPECT_TRUE(ListCount(list) == 0);
  ListInsert(list, &a);
  EXPECT_TRUE(1 == *(int*)ListFirst(list));
}

TEST(ListAddInsert, addBeforeTheHead) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);

  ListFirst(list);
  EXPECT_TRUE(list->currFlag == 0);
  ListPrev(list);
  EXPECT_TRUE(list->currFlag == -1);

  ListAdd(list, &d);
  EXPECT_TRUE(4 == *(int*)ListFirst(list));
}

TEST(ListAddInsert, insertBeforeTheHead) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);

  ListFirst(list);
  EXPECT_TRUE(list->currFlag == 0);
  ListPrev(list);
  EXPECT_TRUE(list->currFlag == -1);

  ListInsert(list, &d);
  EXPECT_TRUE(4 == *(int*)ListFirst(list));
}

TEST(ListAddInsert, addBeyondTheTail) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);

  ListLast(list);
  EXPECT_TRUE(list->currFlag == 0);
  ListNext(list);
  EXPECT_TRUE(list->currFlag == 1);

  ListAdd(list, &d);
  EXPECT_TRUE(1 == *(int*)ListFirst(list));
  EXPECT_TRUE(4 == *(int*)ListLast(list));
}

TEST(ListAddInsert, insertBeyondTheTail) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);

  ListLast(list);
  EXPECT_TRUE(list->currFlag == 0);
  ListNext(list);
  EXPECT_TRUE(list->currFlag == 1);

  ListInsert(list, &d);
  EXPECT_TRUE(1 == *(int*)ListFirst(list));
  EXPECT_TRUE(4 == *(int*)ListLast(list));
}

TEST(ListAddInsert, addAtTheTail) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);

  ListLast(list);
  EXPECT_TRUE(list->currFlag == 0);

  ListAdd(list, &d);
  EXPECT_TRUE(4 == *(int*)ListLast(list));
}

TEST(ListAddInsert, insertAtTheHead) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);

  ListFirst(list);
  EXPECT_TRUE(list->currFlag == 0);

  ListInsert(list, &d);
  EXPECT_TRUE(4 == *(int*)ListFirst(list));
}

