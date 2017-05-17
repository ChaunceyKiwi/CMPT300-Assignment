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
  listPtr->head = (struct ListNode*)(listPtr + sizeof(int) + 2 * sizeof(struct ListNode *));
  listPtr->curr = NULL;
  return listPtr;
}

int ListCount(LIST list) {
  return list.listCount;
}

void *ListFirst(LIST list) {
  list.curr = list.head;
  return list.curr->val;
}

void *ListLast(LIST list) {
  list.curr = list.head + list.listCount - 1;
  return list.curr->val;
}

void *ListNext(LIST list) {
  // Advance list's current item by one
  list.curr++;

  // If not beyond the end of the list, return the item, otherwise NULL
  if (list.curr <= (list.head + list.listCount - 1)) {
    return list.curr->val;
  } else {
    return NULL;
  }
}

void *ListPrev(LIST list) {
  // Back up list's current item by one
  list.curr--;

  // If not beyond the start of the list, return the item, otherwise NULL
  if (list.curr >= list.head) {
    return list.curr->val;
  } else {
    return NULL;
  }
}

int main(void)
{
  LIST *temp = ListCreate();
  printf("%d\n", ListCount(*temp));
  return 0;
}
