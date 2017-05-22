#include "gtest/gtest.h"
#include "list.h"
#include <stdio.h>

// Test1: AppendFirstNextTest
TEST(LIST, AppendFirstNextTest) {
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

// Test2: addAtDesignedPlace
TEST(MaxSubArraySum, addAtDesignedPlace) {
  LIST *list = ListCreate();
  
  int a = 5;
  ListAppend(list, &a);
  
  int b = 6;
  ListAppend(list, &b);

  int c = 7;
  ListAppend(list, &c);

  int d = 8;
  ListAppend(list, &d);
  
  ListFirst(list); // current pointer at head 5
  ListNext(list);  // current pointer at 6
  ListNext(list);  // current pointer at 7
//  ListNext(temp);  // current pointer at tail 8
//  ListNext(temp);  // current pointer beyond tail
  
  int e = 9;
  EXPECT_EQ(0, ListAdd(list, &e));

  EXPECT_EQ(a, *(int*)ListFirst(list));
  EXPECT_EQ(b, *(int*)ListNext(list));
  EXPECT_EQ(c, *(int*)ListNext(list));
  EXPECT_EQ(e, *(int*)ListNext(list));
  EXPECT_EQ(d, *(int*)ListNext(list));
}

// Test3: createAndAdd
TEST(MaxSubArraySum, createAndAdd) {
  LIST *list = ListCreate();
  
  int a = 5;
  ListAdd(list, &a);
  
  EXPECT_EQ(a, *(int*)ListFirst(list));
}

// Test4: remove
TEST(MaxSubArraySum, remove) {
LIST *list = ListCreate();
  
  int a = 5;
  ListAppend(list, &a);
  
  int b = 6;
  ListAppend(list, &b);
  
  int c = 7;
  ListAppend(list, &c);
  
  int d = 8;
  ListAppend(list, &d);
  
  ListFirst(list); // current pointer at head 5
//  ListNext(list);  // current pointer at 6
//  ListNext(list);  // current pointer at 7
//  ListNext(list);  // current pointer at tail 8
//  ListNext(list);  // current pointer beyond tail
  
  EXPECT_EQ(a, *(int*)ListRemove(list));
  EXPECT_EQ(b, *(int*)ListFirst(list));
  EXPECT_EQ(c, *(int*)ListNext(list));
  EXPECT_EQ(d, *(int*)ListNext(list));
}

// Test4: remove
TEST(MaxSubArraySum, search) {
  LIST *list1 = ListCreate();

  // initialization of temp1
  int a = 1;
  ListAppend(list1, &a);

  int b = 2;
  ListAppend(list1, &b);

  int c = 3;
  ListAppend(list1, &c);

  int d = 4;
  ListAppend(list1, &d);

  ListFirst(list1);

  int target1 = 4;
  void* searchRes1 = ListSearch(list1, intEqualTo, &target1);
  EXPECT_EQ(target1, *(int*)searchRes1);

  // int target2 = 5;
  // mvoid* searchRes2 = ListSearch(list1, intEqualTo, &target2);
  // EXPECT_EQ(NULL, *(int*)searchRes2);
}

