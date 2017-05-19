#ifndef list_h
#define list_h

#include <stdio.h>

#define headsPoolSize 20
#define nodesPollSize 1000

typedef struct ListNode_ {
  void *val;
  struct ListNode_ *prev;
  struct ListNode_ *next;
} ListNode;

typedef struct List {
  int len;
  int currFlag; // -1: before head, 0: in the list, 1: beyond tail
  ListNode *head;
  ListNode *tail;
  ListNode *curr;
  struct List *next; // used for freeHeadList
} LIST;

//////////////////////////////////////////////
// Routine functions declaration

// Makes a new, empty list, and returns its reference
// on success. Returns a NULL pointer on failure.
LIST *ListCreate();

// Returns the number of items in list
int ListCount(LIST* list);

// Returns a pointer to the first item in list
// and makes the first item the current item.
void *ListFirst(LIST* list);

// Returns a pointer to the last item in list
// and makes the last item the current one.
void *ListLast(LIST* list);

// Advances list's current item by one, and returns a
// pointer to the new current item. If this operation
// advances the current item beyond the end of the list,
// a NULL pointer is returned.
void *ListNext(LIST* list);

// Backs up list's current item by one, and returns a
// pointer to the new current item. If this operation
// backs up the current item beyond the start of the
// list, a NULL pointer is returned.
void *ListPrev(LIST* list);

// Returns a pointer to the current item in list.
void *ListCurr(LIST* list);

// Adds the new item to list directly after the
// current item, and makes item the current item.
// If the current pointer is before the start of
// the list, the item is added at the start. If the
// current pointer is beyond the end of the list,
// the item is added at the end.
// Returns 0 on success, -1 on failure.
int ListAdd(LIST* list, void* item);

// Adds item to list directly before the current item,
// and makes the new item the current one. If the current
// pointer is before the start of the list, the item is
// added at the start. If the current pointer is beyond
// the end of the list, the item is added at the end.
// Returns 0 on success, -1 on failure.
int ListInsert(LIST* list, void* item);

// Adds item to the end of list, and makes the new item the
// current one. Returns 0 on success, -1 on failure.
int ListAppend(LIST* list, void* item);

// Adds item to the front of list, and makes the new item the
// current one. Returns 0 on success, -1 on failure.
int ListPrepend(LIST* list, void* item);

// Return current item and take it out of list. Make the next item the current one.
void *ListRemove(LIST* list);

// Adds list2 to the end of list1. The current pointer is set to the
// current pointer of list1. List2 no longer exists after the operation.
void ListConcat(LIST* list1, LIST* list2);

// delete list. itemFree is a pointer to a routine that frees an item.
// It should be invoked (within ListFree) as: (*itemFree)(itemToBeFreed);
void ListFree(LIST* list, void *itemFree(LIST* list));

//////////////////////////////////////////////
// Testing functions declaration

void ListPrint(LIST *list1);

#endif /* list_h */
