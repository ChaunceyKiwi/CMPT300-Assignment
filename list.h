#ifndef list_h
#define list_h

#include <stdio.h>

#define headsPoolSize 20
#define nodesPollSize 1000

typedef struct ListNode_ {
  void *val;
  struct ListNode_ *next;
  struct ListNode_ *prev;
} ListNode;

typedef struct List {
  int len;
  ListNode *head;
  ListNode *tail;
  ListNode *curr;
} LIST;

LIST *ListCreate();
void *ListFirst(LIST* list);
void *ListLast(LIST* list);
void *ListNext(LIST* list);
void *ListPrev(LIST* list);
void *ListCurr(LIST* list);
int ListAdd(LIST* list, void* item);
int ListInsert(LIST* list, void* item);
int ListAppend(LIST* list, void* item);
int ListPrepend(LIST* list, void* item);


#endif /* list_h */
