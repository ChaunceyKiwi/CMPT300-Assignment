#include "gtest/gtest.h"
#include "../include/list.h"
#include <stdio.h>

/* unit_test1 focus on testing of ListCreate() */

/* unit_test1_sub1: over allocation */
TEST(ListCreate, overAllocation) {
  int i;
  LIST* list[headsPoolSize];

  for (i = 0; i < headsPoolSize; i++) {
    list[i] = ListCreate();
  }

  EXPECT_EQ(NULL, ListCreate());
  EXPECT_EQ(NULL, ListCreate());

  // clean memory to make sure remaining test won't go wrong
  for (i = 0; i < headsPoolSize; i++) {
    ListFree(list[i], ListRemove);
  }
}

/* unit_test1_sub2: allocatoin and deallocation */
TEST(ListCreate, allocatoinAndDeallocation) {
  int i;
  LIST *list[headsPoolSize];

  // after loop, all heads in heads poll are still free
  for (i = 0; i < headsPoolSize; i++) {
    list[i] = ListCreate();
    ListFree(list[i], ListRemove);
  }

  // After loop, all heads in heads poll are exhausted
  for (i = 0; i < headsPoolSize; i++) {
    list[i] = ListCreate();
  }

  // After free, two heads will be available
  ListFree(list[2], ListRemove);
  ListFree(list[5], ListRemove);

  // After creation, all heads are exhausted
  EXPECT_TRUE(ListCreate() != NULL);
  EXPECT_TRUE(ListCreate() != NULL);
  EXPECT_TRUE(ListCreate() == NULL);

  // clean memory to make sure remaining test won't go wrong
  for (i = 0; i < headsPoolSize; i++) {
    ListFree(list[i], ListRemove);
  }
}

/* unit_test1_ending: make sure no memory leak */
TEST(ListCreate, overAllocation2) {
  int i;
  LIST* list[headsPoolSize];

  for (i = 0; i < headsPoolSize; i++) {
    list[i] = ListCreate();
  }

  // clean memory to make sure remaining test won't go wrong
  for (i = 0; i < headsPoolSize; i++) {
    ListFree(list[i], ListRemove);
  }
}
