#include <stdio.h>
#include <stdlib.h>
#define headsPoolSize 20
#define nodesPollSize 1000

typedef struct ListNode_ {
  void *val;
  struct ListNode *next;
} ListNode;

typedef struct List {
  int len;
  ListNode *head;
  ListNode *tail;
  ListNode *curr;
} LIST;

LIST headsPool[headsPoolSize];
int freeHeadIndex = 0;
ListNode nodesPool[nodesPollSize];
int freeNodeIndex = 0;

LIST *ListCreate() {
  LIST *list= &headsPool[freeHeadIndex++];
  list->len = 0;
  list->head = NULL;
  list->tail = NULL;
  list->curr = NULL;
  return list;
}

int main(void)
{
  LIST *temp = ListCreate();
  return 0;
}
