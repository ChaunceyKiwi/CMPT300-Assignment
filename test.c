#include "gtest/gtest.h"
#include "list.h"
#include <stdio.h>

// Test1: AppendFirstNextTest
TEST(MaxSubArraySum, AppendFirstNextTest) {
  LIST *list1 = ListCreate();
  LIST *list2 = ListCreate();
  
  int a = 5;
  int b = 6;
  int c = 7;
  int d = 8;
  int e = 9;

  ListAppend(list1, &a);
  ListAppend(list1, &b);
  ListAppend(list1, &c);
  ListAppend(list1, &d);

  ListAppend(list2, &b);
  ListAppend(list2, &c);
  ListAppend(list2, &d);
  ListAppend(list2, &e);

  EXPECT_EQ(a, *(int*)ListFirst(list1));
  EXPECT_EQ(b, *(int*)ListNext(list1));
  EXPECT_EQ(c, *(int*)ListNext(list1));
  EXPECT_EQ(d, *(int*)ListNext(list1));

  EXPECT_EQ(b, *(int*)ListFirst(list2));
  EXPECT_EQ(c, *(int*)ListNext(list2));
  EXPECT_EQ(d, *(int*)ListNext(list2));
  EXPECT_EQ(e, *(int*)ListNext(list2));
}
