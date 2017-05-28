#include "gtest/gtest.h"
#include "../include/list.h"
#include <stdio.h>

/* unit_test3 focus on testing of ListNext() and ListPrev() */

/* unit_test3_sub1 normal case and beyond the tail or before the head after operation */
TEST(ListNextPrev, normalCase) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  EXPECT_TRUE(1 == *(int*)ListFirst(list));
  EXPECT_TRUE(2 == *(int*)ListNext(list));
  EXPECT_TRUE(3 == *(int*)ListNext(list));
  EXPECT_TRUE(4 == *(int*)ListNext(list));
  EXPECT_TRUE(NULL == ListNext(list));  /* can be multiple same lines here */
  EXPECT_TRUE(NULL == list->curr);
  EXPECT_TRUE(1 == list->currFlag);

  EXPECT_TRUE(4 == *(int*)ListLast(list));
  EXPECT_TRUE(3 == *(int*)ListPrev(list));
  EXPECT_TRUE(2 == *(int*)ListPrev(list));
  EXPECT_TRUE(1 == *(int*)ListPrev(list));
  EXPECT_TRUE(NULL == ListPrev(list));  /* can be multiple same lines here */
  EXPECT_TRUE(NULL == list->curr);
  EXPECT_TRUE(-1 == list->currFlag);
}

/* unit_test3_sub1 before the head in the beginning */
TEST(ListNextPrev, beyondTheList) {
  int a = 1, b = 2, c = 3, d = 4;
  LIST* list = ListCreate();
  ListAppend(list, &a);
  ListAppend(list, &b);
  ListAppend(list, &c);
  ListAppend(list, &d);

  /* Test the prev before the head*/
  ListFirst(list);
  EXPECT_TRUE(NULL == ListPrev(list)); /* can be multiple same lines here */
  EXPECT_TRUE(NULL == ListPrev(list)); /* can be multiple same lines here */
  EXPECT_TRUE(NULL == ListPrev(list)); /* can be multiple same lines here */
  EXPECT_TRUE(NULL == list->curr);
  EXPECT_TRUE(-1 == list->currFlag);

  /* Now the current is before the head, the next will be head no matter how many ListPrev() before */
  EXPECT_TRUE(*(int*)ListNext(list) == 1);
  EXPECT_TRUE(list->curr = list->head);

  /* Test the next after the tail*/
  ListLast(list);
  EXPECT_TRUE(NULL == ListNext(list)); /* can be multiple same lines here */
  EXPECT_TRUE(NULL == ListNext(list)); /* can be multiple same lines here */
  EXPECT_TRUE(NULL == ListNext(list)); /* can be multiple same lines here */
  EXPECT_TRUE(NULL == list->curr);
  EXPECT_TRUE(1 == list->currFlag);

  /* Now the current is beyond the tail, the prev will be tail no matter how many ListNext() before */
  EXPECT_TRUE(*(int*)ListPrev(list) == 4);
  EXPECT_TRUE(list->curr = list->tail);
}

TEST(ListNextPrev, emptyList) {
  LIST* list = ListCreate();

  EXPECT_TRUE(NULL == ListNext(list));
  EXPECT_TRUE(NULL == ListPrev(list));
}

