/*------------------------------------------------------
 *
 *  list.h
 *
 *  This file contains the declarations of routine functions,
 *  the definitions of structs and global variables
 *
 *  Name         : Chauncey Liu
 *  Student ID   : 301295771
 *  SFU username : cla284
 *  Course       : CMPT 300 Operating Systems I, Summer 2017
 *  Instructor   : Harinder Khangura
 *  TA           : Amineh Dadsetan
 *
 *  Created by Chauncey on 2017-05-22.
 *  Copyright (c) 2017 Chauncey. All rights reserved.
 *
 *------------------------------------------------------
 */

#ifndef OS_ASS1_LIST_H_
#define OS_ASS1_LIST_H_

#include <stdio.h>

#define headsPoolSize 10     /* The total number of heads available */
#define nodesPollSize 100   /* The total number of nodes available */

typedef struct ListNode_ {
  void *val;
  struct ListNode_ *prev;
  struct ListNode_ *next;
} ListNode;

typedef struct List {
  int len;
  int currFlag;   /* -1: before head, 0: in the list, 1: beyond tail */
  ListNode *head;
  ListNode *tail;
  ListNode *curr;
  struct List *next; /* used for freeHeadList */
} LIST;

/***********************************************************
*   Routine functions declaration
*/

/* Makes and returns a new, empty list */
LIST *ListCreate();

/* Returns the number of items in list */
int ListCount(LIST *list);

/* Returns a pointer to the first item in list,
 * and makes the first item the current item. */
void *ListFirst(LIST *list);

/* Returns a pointer to the last item in list,
 * and makes the last item the current one. */
void *ListLast(LIST *list);

/* Advances list's current item by one, and
 * returns a pointer to the new current item. */
void *ListNext(LIST *list);

/* Backs up list's current item by one, and
 * returns a pointer to the new current item. */
void *ListPrev(LIST *list);

/* Returns a pointer to the current item in list. */
void *ListCurr(LIST *list);

/* Adds the new item to list directly after the
 * current item, and makes item the current item. */
int ListAdd(LIST *list, void *item);

/* Adds item to list directly before the current item,
 * and makes the new item the current one. */
int ListInsert(LIST *list, void *item);

/* Adds item to the end of list, and makes the new item the current one. */
int ListAppend(LIST *list, void *item);

/* Adds item to the front of list, and makes the new item the current one. */
int ListPrepend(LIST *list, void *item);

/* Returns current item and take it out of list. */
void *ListRemove(LIST *list);

/* Adds list2 to the end of list1. */
void ListConcat(LIST *list1, LIST *list2);

/* Deletes list */
void ListFree(LIST *list, void (*itemFree)(void*));

/* Returns last item and take it out of list. */
void *ListTrim(LIST *list);

/* Searches an item in the list */
void *ListSearch(LIST *list, int (*comparator)(void*, void*), void* comparisonArg);

/***********************************************************
*   Helper functions declaration
*/

/* Sets up heads pool and nodes pool */
void setUpPools();

/* Finds and returns a free list */
LIST* allocateList();

/* Updates the value of members in a list */
void updateList(LIST* list, int len, int currFlag, ListNode* head, ListNode* tail, ListNode* curr);

/* Puts a list back to free list pool */
void reclaimListHead(LIST* list);

/* Finds and returns a free list node */
ListNode* allocateNode();

/* Updates the value of members in a list node */
void updateListNode(ListNode* listNode, void* item, ListNode* prev, ListNode* next);

/* Puts a list node back to free list node pool */
void reclaimNode(ListNode* listNode);

/***********************************************************
*   Testing functions declaration
*/

void ListPrint(LIST *list);

#endif /* OS_ASS1_LIST_H_ */
