/*------------------------------------------------------
 *
 *  list.c
 *
 *  This file contain the routine functions to implement
 *  the LIST abstract data type
 *
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

#include "../include/list.h"


/***********************************************************
*    Global initialization
*/

LIST headsPool[headsPoolSize];      /* Statically pre-allocated array pool for heads */
int freeHeadIndex = 0;              /* Used to indicate the first head in headsPool that has never been used before */
LIST *freeHeadList = NULL;          /* Used to indicate the free heads that are recycled after removal */
ListNode nodesPool[nodesPollSize];  /* Statically pre-allocated array pool for nodes */
int freeNodeIndex = 0;              /* Used to indicate the first head in nodesPool that has never been used before */
ListNode *freeNodeList = NULL;      /* Used to indicate the free nodes that are recycled after removal */

/***********************************************************
*   Routine functions implementation
*/


/**
 * Makes and returns a new, empty list
 * @return Returns its reference on success. Returns a NULL pointer on failure.
 */
LIST *ListCreate() {
  LIST *list= allocateList();
  if (list != NULL) {
    // Initialize the current pointer before the header
    updateList(list, 0, -1, NULL, NULL, NULL);
  }
  return list;
}


/**
 * Returns the number of items in list
 * @param list the list to be counted
 * @return the number of items in list
 */
int ListCount(LIST* list) {
  return list->len;
}

/**
 * Makes the first item the current item
 * @param list the current pointer of which is
 * to be located at the first item
 * @return a pointer to the first item in list
 */
void *ListFirst(LIST* list) {
  list->curr = list->head;
  return ListCurr(list);
}

/**
 * Makes the last item the current item
 * @param list the current pointer of which is
 * to be located at the last item
 * @return a pointer to the last item in list
 */
void *ListLast(LIST* list) {
  list->curr = list->tail;
  return ListCurr(list);
}

/**
 * Advances list's current item by one
 * @param list the list to be advanced
 * @return a pointer to the new current item. If
 * this operation advances the current item beyond
 * the end of the list, a NULL pointer is returned
 */
void *ListNext(LIST* list) {
  // If the current pointer points out of the list, return NULL
  if (list->curr == NULL) {
    return NULL;
  }

  list->curr = list->curr->next;
  void *res = ListCurr(list);

  // set flag when the current pointer goes beyond the tail
  if (res == NULL) {
    list->currFlag = 1;
  }

  return res;
}

/**
 * Backs up list's current item by one
 * @param list the list to be backed up
 * @return a pointer to the new current item. If
 * this operation backs up the current item beyond
 * the start of the list, a NULL pointer is returned.
 */
void *ListPrev(LIST* list) {
  // If the current pointer points out of the list, return NULL
  if (list->curr == NULL) {
    return NULL;
  }

  list->curr = list->curr->prev;
  void *res = ListCurr(list);

  // set flag when the current pointer goes before the head
  if (res == NULL) {
    list->currFlag = -1;
  }

  return res;
}

/**
 * Returns a pointer to the current item in list
 * @param list the current pointer of which is to be returned
 * @return A pointer to the current item in list.
 */
void *ListCurr(LIST* list) {
  if (list->curr != NULL) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

/**
 * Adds the new item to list directly after the current item
 * @param list list to be added an item
 * @param item item to be added to list
 * @return 0 on success, -1 on failure
 */
int ListAdd(LIST* list, void* item) {
  // Success case 1
  if (list->curr == NULL && list->currFlag != 0) {
    // if the current pointer is before the head
    if (list->currFlag == -1) {
      return ListPrepend(list, item);
    }
    // if the current pointer is beyond the tail
    else {
      return ListAppend(list, item);
    }
  }

  // Fail case 1: Current pointer is not set yet
  if (list->curr == NULL) {
    return -1;
  }

  // Try allocating space for new list node
  ListNode* temp = list->curr;
  list->curr = allocateNode();

  // Fail case 2: No more free node in nodes pool
  if (list->curr == NULL) {
    return -1;
  }

  // Success case 2
  else {
    updateListNode(list->curr, item, temp, temp->next);

    // If current pointer is at the tail
    if (temp->next == NULL) {
      list->tail = list->curr;
    } else {
      temp->next->prev = list->curr;
    }

    temp->next = list->curr;
    list->len++;
    return 0;
  }
}

/**
 * Adds item to list directly before the current item
 * @param list list to be inserted
 * @param item item to insert
 * @return 0 on success, -1 on failure
 */
int ListInsert(LIST* list, void* item) {
  // Success case 1
  if (list->curr == NULL && list->currFlag != 0) {
    // if the current pointer is before the head
    if (list->currFlag == -1) {
      return ListPrepend(list, item);
    }
    // if the current pointer is beyond the tail
    else {
      return ListAppend(list, item);
    }
  }

  // Fail case 1: Current pointer is not set yet
  if (list->curr == NULL) {
    return -1;
  }

  // Try allocating space for new list node
  ListNode* temp = list->curr;
  list->curr = allocateNode();

  // Fail case 2: No more free node in nodes pool
  if (list->curr == NULL) {
    return -1;
  }

  // Success case 2
  else {
    updateListNode(list->curr, item, temp->prev, temp);

    // If current pointer is at the head
    if (temp->prev != NULL) {
      temp->prev->next = list->curr;
    } else {
      list->head = list->curr;
    }

    temp->prev = list->curr;
    list->len++;
    return 0;
  }
}

/**
 * Adds item to the end of list
 * @param list the list to be appended
 * @param item the item to append
 * @return 0 on success, -1 on failure
 */
int ListAppend(LIST* list, void* item) {
  // Try allocating space for new list node
  list->curr = allocateNode();

  // Fail case: No more free nodes in nodes pool
  if (list->curr == NULL) {
    return -1;
  }

  // Success case
  else {
    // If the list is empty
    if (list->len == 0) {
      updateListNode(list->curr, item, NULL, NULL);
      updateList(list, list->len + 1, 0, list->curr, list->curr, list->curr);
      return 0;
    }

    // If the list is not empty
    else {
      updateListNode(list->curr, item, list->tail, NULL);
      list->tail->next = list->curr;
      list->tail = list->curr;
      list->len++;
      return 0;
    }
  }
}

/**
 * Adds item to the front of list
 * @param list the list to be prepended
 * @param item the item to prepend
 * @return 0 on success, -1 on failure
 */
int ListPrepend(LIST* list, void* item) {
  // Try allocating space for new list node
  list->curr = allocateNode();

  // Fail case: No more free nodes in nodes pool
  if (list->curr == NULL) {
    return -1;
  }

  // Success case
  else {
    // If the list is empty
    if (list->len == 0) {
      updateListNode(list->curr, item, NULL, NULL);
      updateList(list, list->len + 1, 0, list->curr, list->curr, list->curr);
      return 0;
    }

    // If the list is not empty
    else {
      updateListNode(list->curr, item, NULL, list->head);
      list->head->prev = list->curr;
      list->head = list->curr;
      list->len++;
      return 0;
    }
  }
}

/**
 * Take current item out of list
 * @param list the current item of which is to be removed
 * @return current item of the list
 */
void *ListRemove(LIST* list) {
  // If current list pointer points out of the list
  if (list->curr == NULL) {
    return NULL;
  } else {
    ListNode* temp = list->curr;
    void* item = temp->val;

    // Current list pointer points at the only node in the list
    if (list->curr == list->head && list->curr == list->tail) {
      list->head = NULL;
      list->tail = NULL;
      list->curr = NULL;
      list->currFlag = -1;
    }

    // Current list pointer points at head
    else if (list->curr == list->head) {
      temp->next->prev = NULL;
      list->head = temp->next;
      list->curr = list->head;
    }

    // Current list pointer points at tail
    else if (list->curr == list->tail) {
      temp->prev->next = NULL;
      list->tail = temp->prev;
      list->curr = list->tail;
    }

    // Current list pointer points at middle item
    else {
      temp->prev->next = temp->next;
      temp->next->prev = temp->prev;
      list->curr = temp->next;
    }

    list->len--;
    FreeNode(temp);
    return item;
  }
}

/**
 * Adds list2 to the end of list1.
 * @param list1 the list to be concatenate
 * @param list2 the list to concatenate
 */
void ListConcat(LIST* list1, LIST* list2) {
  // Do concatenation only when both of the lists are not NULL
  if (list1 != NULL && list2 != NULL) {
    list1->tail->next = list2->head;
    list2->head->prev = list1->tail;
    FreeList(list2);
  }
}

/**
 * Deletes list.
 * @param list the list to be deleted
 * @param itemFree a pointer to a routine that frees an item.
 */
void ListFree(LIST* list, void *itemFree(LIST*)) {
  ListFirst(list);
  while(list->curr != NULL) {
    (*itemFree)(list);
  }
  FreeList(list);
}

/**
 * Take last item out of list.
 * @param list the list to be trimmed
 * @return last item item of the original list
 */
void *ListTrim(LIST* list) {
  ListLast(list);
  return ListRemove(list);
}

/**
 * Search item in the list
 * @param list the list to be searched on
 * @param comparator a pointer to a routine that takes
 * as its first argument an item pointer, and as its
 * second argument comparisonArg.
 * @param comparisonArg the argument for comparator
 * @return If a match is found, the current pointer is left at
 * the matched item and the pointer to that item is returned.
 * If no match isfound, the current pointer is left beyond
 * the end of the list and a NULL pointer is returned.
 */
void *ListSearch(LIST* list, int comparator(LIST*, void*), void* comparisonArg) {
  while(list->curr != NULL) {
    // If item if found
    if (comparator(list, comparisonArg) == 1) {
      return list->curr->val;
    }

    // If not found
    else {
      list->curr = list->curr->next;
    };
  }

  // The current pointer is left beyond the end of the list
  list->currFlag = 1;
  return NULL;
}

/***********************************************************
*   Comparator routine functions declaration
*/

int intEqualTo(LIST* list, void* comparisonArg) {
  return *(int*)(list->curr->val) == *(int*)(comparisonArg);
}

/***********************************************************
*   Helper functions implementation
*/

void updateListNode(ListNode* listNode, void* item, ListNode* prev, ListNode* next) {
  listNode->val = item;
  listNode->prev = prev;
  listNode->next = next;
}

void updateList(LIST* list, int len, int currFlag, ListNode* head, ListNode* tail, ListNode* curr) {
  list->len = len;
  list->currFlag = currFlag;
  list->head = head;
  list->tail = tail;
  list->curr = curr;
}

LIST* allocateList() {
  if (freeHeadList == NULL) {
    if (freeHeadIndex < headsPoolSize) {
      return &headsPool[freeHeadIndex++];
    } else {
      return NULL;
    }
  } else {
    LIST *listFree = freeHeadList;
    freeHeadList = freeHeadList->next;
    return listFree;
  }
}

ListNode* allocateNode() {
  if (freeNodeList == NULL) {
    if (freeNodeIndex < nodesPollSize) {
      return &nodesPool[freeNodeIndex++];
    } else {
      return NULL;
    }
  } else {
    ListNode *nodeFree = freeNodeList;
    freeNodeList = freeNodeList->next;
    return nodeFree;
  }
}

void FreeList(LIST* list) {
  list->next = NULL;

  if (freeHeadList == NULL) {
    freeHeadList = list;
  } else {
    list->next = freeHeadList;
    freeHeadList = list;
  }
}

void FreeNode(ListNode* listNode) {
  listNode->prev = NULL;
  listNode->val = NULL;
  listNode->next = NULL;

  if (freeNodeList == NULL) {
    freeNodeList = listNode;
  } else {
    listNode->next = freeNodeList;
    freeNodeList = listNode;
  }
}

/***********************************************************
*   Testing functions implementation
*/

void ListPrint(LIST *list) {
  ListNode* iter = list->head;
  while(iter != NULL) {
    printf("%d ", *(int*)iter->val);
    iter = iter->next;
  }
  printf("\n");
}
