#include <stdio.h>
#include <stdlib.h>
#define headsSize 20
#define listSize 50

struct ListNode {
  void *val;
  struct ListNode *next;
};

typedef struct List {
  int listCount;
  struct ListNode *head;
  struct ListNode *curr;
  struct ListNode listNodes[listSize];
} LIST;

// Statically allocate memory for headsArray
LIST headsArray[headsSize];
int headsCount = 0;

LIST *ListCreate() {
  LIST *listPtr = &headsArray[headsCount++];
  listPtr->listCount = 0;
  listPtr->curr = NULL;
  return listPtr;
}

int ListCount(LIST* list) {
  return list->listCount;
}

void *ListFirst(LIST* list) {
  list->curr = list->head;
  return list->curr->val;
}

void *ListLast(LIST* list) {
  list->curr = list->head + list->listCount - 1;
  return list->curr->val;
}

void *ListNext(LIST* list) {
  // Advance list's current item by one
  list->curr++;

  // If not beyond the end of the list, return the item, otherwise NULL
  if (list->curr <= (list->head + list->listCount - 1)) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

void *ListPrev(LIST* list) {
  // Back up list's current item by one
  list->curr--;

  // If not beyond the start of the list, return the item, otherwise NULL
  if (list->curr >= list->head) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

void *ListCurr(LIST* list) {
  // If the current pointer is before or beyond the list, return NULL
  if ((list->curr <= (list->head + list->listCount - 1)) ||
      (list->curr >= list->head)) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

// TO-DO
int ListAdd(LIST* list, void* item) {
  return 0;
}

// TO-DO
int ListInsert(LIST* list, void* item) {
  return 0;
}

// TO-DO
int ListAppend(LIST* list, void* item) {

  if (list->listCount == 0) {
    list->listNodes[0].val = item;
    list->listNodes[0].next = NULL;
    list->curr = &list->listNodes[0];
    list->head = list->curr;
    list->listCount++;
    return 0;
  } else {
    list->listNodes[list->listCount].val = item;
    list->listNodes[list->listCount].next = NULL;
    list->listCount++;
    ListLast(list);
    return 0;
  }

  return -1;
}

// TO-DO
int ListPrepend(LIST* list, void* item) {

  if (list->listCount == 0) {
    list->listNodes[0].val = item;
    list->listNodes[0].next = NULL;
    list->curr = &list->listNodes[0];
    list->head = list->curr;
    list->listCount++;
    return 0;
  } else {
    list->listNodes[list->listCount].val = item;
    list->listNodes[list->listCount].next = NULL;
    list->listCount++;
    ListLast(list);
    return 0;
  }

  return -1;
}

int main(void)
{
  LIST *temp = ListCreate();
  int a = 5;
  ListAppend(temp, &a);
  int b = 6;
  ListAppend(temp, &b);
  int c = 7;
  ListAppend(temp, &c);


  printf("%d\n", *(int*)ListFirst(temp));
  printf("%d\n", *(int*)ListNext(temp));
  printf("%d\n", *(int*)ListNext(temp));

  return 0;
}
