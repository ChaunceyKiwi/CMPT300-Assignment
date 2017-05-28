#include "gtest/gtest.h"
#include "../include/list.h"
#include <stdio.h>

/* unit_test2 focus on testing of ListFirst() and ListLast() */

/* unit_test2_sub1 normal case */
TEST(ListCreate, normalCase) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  EXPECT_TRUE(1 == *(int*)ListFirst(list));
  EXPECT_TRUE(4 == *(int*)ListLast(list));
}

TEST(ListCreate, emptyList) {
  LIST* list = ListCreate();

  EXPECT_TRUE(NULL == ListFirst(list));
  EXPECT_TRUE(NULL == ListLast(list));
  EXPECT_TRUE(NULL == list->curr);
}
