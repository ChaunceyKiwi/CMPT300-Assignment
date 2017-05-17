#include <stdio.h>
#include <stdlib.h>
#define headsMaxSize 20
#define listMaxSize 50

struct ListNode {
  void *val;
};

typedef struct List {
  int listCount;
  struct ListNode *head;
  struct ListNode *curr;
  struct ListNode listNodes[listMaxSize];
} LIST;

// Statically allocate memory for headsArray
LIST headsArray[headsMaxSize];
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
  // Check if current pointer is NULL
  if (list->curr == NULL) {
    return -1;
  }

  // Check if nodes are exhausted
  else if (list->listCount >= listMaxSize) {
    return -1;
  }

  else {
    for (struct ListNode *iter = &list->listNodes[list->listCount];
         iter > list->curr + 1; iter--) {
      *iter = *(iter - 1);
    }

    (list->curr + 1)->val = item;
    list->listCount++;
    list->curr++;
    return 0;
  }
}

// TO-DO
int ListInsert(LIST* list, void* item) {
  // Check if current pointer is NULL
  if (list->curr == NULL) {
    return -1;
  }

  // Check if nodes are exhausted
  else if (list->listCount >= listMaxSize) {
    return -1;
  }

  else {
    for (struct ListNode *iter = &list->listNodes[list->listCount];
         iter > list->curr; iter--) {
      *iter = *(iter - 1);
    }

    list->curr->val = item;
    list->listCount++;
    return 0;
  }
}

int ListAppend(LIST* list, void* item) {
  // Check if nodes are exhausted
  if (list->listCount >= listMaxSize) {
    return -1;
  }

  // If the list is empty
  if (list->listCount == 0) {
    list->listNodes[0].val = item;
    list->curr = &list->listNodes[0];
    list->head = list->curr;
    list->listCount++;
    return 0;
  }

  // If the list is not empty
  else {
    list->listNodes[list->listCount].val = item;
    list->listCount++;
    ListLast(list);
    return 0;
  }

  return -1;
}

int ListPrepend(LIST* list, void* item) {
  // Check if nodes are exhausted
  if (list->listCount >= listMaxSize) {
    return -1;
  }

  // If the list is empty
  if (list->listCount == 0) {
    list->listNodes[0].val = item;
    list->curr = &list->listNodes[0];
    list->head = list->curr;
    list->listCount++;
    return 0;
  }

  // If the list is not empty
  else {
    for (struct ListNode *iter = &list->listNodes[list->listCount];
         iter >= &list->listNodes[1]; iter--) {
      *iter = *(iter - 1);
    }

    list->listNodes[0].val = item;
    list->listCount++;
    ListFirst(list);
    return 0;
  }

  return -1;
}

int main(void)
{
  LIST *temp = ListCreate();
  int a = 5;
  ListPrepend(temp, &a);
  int b = 6;
  ListPrepend(temp, &b);
  int c = 7;
  ListPrepend(temp, &c);
  int d = 8;
  ListInsert(temp, &d);

  printf("%d\n", *(int*)ListFirst(temp));
  printf("%d\n", *(int*)ListNext(temp));
  printf("%d\n", *(int*)ListNext(temp));
  printf("%d\n", *(int*)ListNext(temp));

  return 0;
}
