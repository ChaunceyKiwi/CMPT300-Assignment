/*------------------------------------------------------
 *
 *  list.c
 *
 *  This file contains the routine functions to implement
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
#include <assert.h>

/***********************************************************
*    Global initialization
*/

LIST headsPool[headsPoolSize];      /* Statically pre-allocated array pool for heads */
LIST *freeHeadList = NULL;          /* Used to indicate the free heads that are recycled after removal */
ListNode nodesPool[nodesPollSize];  /* Statically pre-allocated array pool for nodes */
ListNode *freeNodeList = NULL;      /* Used to indicate the free nodes that are recycled after removal */
int isPoolsSetUp = 0;               /* Pool is set up at the very first time that ListCreate() is called */

/***********************************************************
*   Routine functions implementation
*/


/**
 * Makes and returns a new, empty list
 * @return Its reference on success. A NULL pointer on failure.
 */
LIST *ListCreate() {
  if (!isPoolsSetUp) {
    setUpPools();
    isPoolsSetUp = 1;
  }

  LIST *list= allocateList();

  if (list != NULL) {
    /* Initialize the current pointer before the header */
    updateList(list, 0, -1, NULL, NULL, NULL);
  } else {
    printf("Failure: All heads are exhausted.\n");
  }

  return list;
}

/**
 * Returns the number of items in list
 * @param list the target list
 * @return the number of items in list
 */
int ListCount(LIST* list) {
  assert(list != NULL);
  return list->len;
}

/**
 * Makes the first item the current item
 * @param list the target list
 * @return a pointer to the first item in list
 */
void *ListFirst(LIST* list) {
  assert(list != NULL);
  list->curr = list->head;
  return ListCurr(list);
}

/**
 * Makes the last item the current item
 * @param list the target list
 * @return a pointer to the last item in list
 */
void *ListLast(LIST* list) {
  assert(list != NULL);
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
  assert(list != NULL);
  assert(list->curr != NULL || list->currFlag != 0);

  if (list->curr == NULL) {
    /* If the current pointer points before the head of the list */
    if (list->currFlag == -1) {
      return ListFirst(list);
    }

    /* If the current pointer points beyond the tail of the list */
    else if (list->currFlag == 1) {
      return NULL;
    }
  }

  list->curr = list->curr->next;
  void *res = ListCurr(list);

  /* set flag when the current pointer goes beyond the tail */
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
  assert(list != NULL);
  assert(list->curr != NULL || list->currFlag != 0);

  if (list->curr == NULL) {
    /* If the current pointer points beyond the tail of the list */
    if (list->currFlag == 1) {
      return ListLast(list);
    }

    /* If the current pointer points before the head of the list */
    else if (list->currFlag == -1){
      return NULL;
    }
  }

  list->curr = list->curr->prev;
  void *res = ListCurr(list);

  /* set flag when the current pointer goes before the head */
  if (res == NULL) {
    list->currFlag = -1;
  }

  return res;
}

/**
 * Returns a pointer to the current item in list
 * @param list the target list
 * @return A pointer to the current item in list.
 */
void *ListCurr(LIST* list) {
  assert(list != NULL);
  if (list->curr != NULL) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

/**
 * Adds the new item to list directly after the current item
 * @param list the list to be added an item
 * @param item item to be added to list
 * @return 0 on success, -1 on failure
 */
int ListAdd(LIST* list, void* item) {
  assert(list != NULL && item != NULL);
  assert(list->curr != NULL || list->currFlag != 0);

  /* If list is empty */
  if (list->curr == NULL) {
    /* if the current pointer is before the head */
    if (list->currFlag == -1) {
      return ListPrepend(list, item);
    }
    /* if the current pointer is beyond the tail */
    else if (list->currFlag == 1){
      return ListAppend(list, item);
    }
  }

  /* Try allocating space for new list node */
  ListNode* temp = list->curr;
  list->curr = allocateNode();

  /* Check if nodes are exhausted */
  if (list->curr == NULL) {
    printf("Failure: All nodes are exhausted.\n");
    return -1;
  } else {
    updateListNode(list->curr, item, temp, temp->next);

    /* Check if current pointer is at the tail */
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
 * @param list the list to be inserted
 * @param item item to insert
 * @return 0 on success, -1 on failure
 */
int ListInsert(LIST* list, void* item) {
  assert(list != NULL && item != NULL);
  assert(list->curr != NULL || list->currFlag != 0);

  /* If list is empty */
  if (list->curr == NULL) {
    /* if the current pointer is before the head */
    if (list->currFlag == -1) {
      return ListPrepend(list, item);
    }
    /* if the current pointer is beyond the tail */
    else if (list->currFlag == 1){
      return ListAppend(list, item);
    }
  }

  /* Try allocating space for new list node */
  ListNode* temp = list->curr;
  list->curr = allocateNode();

  /* Check if nodes are exhausted */
  if (list->curr == NULL) {
    printf("Failure: All nodes are exhausted.\n");
    return -1;
  } else {
    updateListNode(list->curr, item, temp->prev, temp);

    /* Check if current pointer is at the head */
    if (temp->prev == NULL) {
      list->head = list->curr;
    } else {
      temp->prev->next = list->curr;
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
  assert(list != NULL && item != NULL);

  /* Check if the list is empty */
  if (list->tail == NULL) {
    /* Try allocating space for new list node */
    list->curr = allocateNode();

    /* Check if nodes are exhausted */
    if (list->curr == NULL) {
      printf("Failure: All nodes are exhausted.\n");
      return -1;
    } else {
      updateListNode(list->curr, item, NULL, NULL);
      updateList(list, list->len + 1, 0, list->curr, list->curr, list->curr);
      return 0;
    }
  } else {
    ListLast(list);
    return ListAdd(list, item);
  }
}

/**
 * Adds item to the front of list
 * @param list the list to be prepended
 * @param item the item to prepend
 * @return 0 on success, -1 on failure
 */
int ListPrepend(LIST* list, void* item) {
  assert(list != NULL && item != NULL);

  /* Check if the list is empty */
  if (list->head == NULL) {
    return ListAppend(list, item);
  } else {
    ListFirst(list);
    return ListInsert(list, item);
  }
}

/**
 * Take current item out of list
 * @param list the current item of which is to be removed
 * @return current item of the list
 */
void *ListRemove(LIST* list) {
  assert(list != NULL);

  /* If current list pointer points out of the list */
  if (list->curr == NULL) {
    return NULL;
  } else {
    ListNode* temp = list->curr;
    void* item = temp->val;

    /* If current node is the last node to remove */
    if (list->curr == list->head && list->curr == list->tail) {
      list->head = NULL;
      list->tail = NULL;
      list->curr = NULL;
      list->currFlag = -1;
    }

    /* Current list pointer points at head */
    else if (list->curr == list->head) {
      temp->next->prev = NULL;
      list->head = temp->next;
      list->curr = list->head;
    }

    /* Current list pointer points at tail */
    else if (list->curr == list->tail) {
      temp->prev->next = NULL;
      list->tail = temp->prev;
      list->curr = list->tail;
    }

    /* Current list pointer points at middle item */
    else {
      temp->prev->next = temp->next;
      temp->next->prev = temp->prev;
      list->curr = temp->next;
    }

    list->len--;
    reclaimNode(temp);
    return item;
  }
}

/**
 * Adds list2 to the end of list1.
 * @param list1 the list to be concatenate
 * @param list2 the list to concatenate
 */
void ListConcat(LIST* list1, LIST* list2) {
  /* Do concatenation only when both of the lists are not NULL */
  if (list1 != NULL && list2 != NULL) {
    list1->tail->next = list2->head;
    list2->head->prev = list1->tail;
    list1->len += list2->len;

    /* Since all nodes in list2 is moved to list1, the nodes should not be freed */
    /* Thus here we only need to free the list head and retain the nodes */
    reclaimListHead(list2);
  }
}

/**
 * Deletes a list.
 * @param list the list to be deleted
 * @param itemFree a pointer to a routine that frees an item.
 */
void ListFree(LIST* list, void itemFree(void*)) {
  assert(list != NULL && itemFree != NULL);

  /* 1. Free all nodes in the list */
  ListFirst(list);
  while(list->curr != NULL) {
    itemFree(list->curr->val);
    ListRemove(list);
  }

  /* 2. Reclaim the list head */
  reclaimListHead(list);
}

/**
 * Takes last item out of list.
 * @param list the list to be trimmed
 * @return last item item of the original list
 */
void *ListTrim(LIST* list) {
  assert(list != NULL);
  ListLast(list);
  return ListRemove(list);
}

/**
 * Searches item in the list
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
    /* If item if found */
    if (comparator(list, comparisonArg) == 1) {
      return list->curr->val;
    }

    /* If not found */
    else {
      list->curr = list->curr->next;
    };
  }

  /* The current pointer is left beyond the end of the list */
  list->currFlag = 1;
  return NULL;
}

/***********************************************************
*   Helper functions implementation
*/

/**
 * Sets up heads pool and nodes pool
 */
void setUpPools() {
  int i;

  /* Connect items in pools up */
  for (i = 0; i < headsPoolSize - 1; i++) {
    headsPool[i].next = &headsPool[i+1];
  }
  headsPool[i].next = NULL;

  for (i = 0; i < nodesPollSize - 1; i++) {
    nodesPool[i].next = &nodesPool[i+1];
  }
  nodesPool[i].next = NULL;

  /* Set free list */
  if (headsPoolSize > 0) {
    freeHeadList = &headsPool[0];
  }

  if (nodesPollSize > 0) {
    freeNodeList = &nodesPool[0];
  }
}

/**
 * Finds and returns a free list
 * @return Returns its reference on success. Returns a NULL pointer on failure.
 */
LIST* allocateList() {
  if (freeHeadList != NULL) {
    LIST *listFree = freeHeadList;
    freeHeadList = freeHeadList->next;
    return listFree;
  } else {
    return NULL;
  }
}

/**
 * Updates the value of members in a list
 * @param list the list to be updated
 * @param len the value of list->len after updating
 * @param currFlag the value of list->currFlag after updating
 * @param head the value of list->head after updating
 * @param tail the value of list->tail after updating
 * @param curr the value of list->curr after updating
 */
void updateList(LIST* list, int len, int currFlag, ListNode* head, ListNode* tail, ListNode* curr) {
  list->len = len;
  list->currFlag = currFlag;
  list->head = head;
  list->tail = tail;
  list->curr = curr;
}

/**
 * Puts a list back to free list pool
 * @param list the list to be free
 */
void reclaimListHead(LIST* list) {
  list->next = NULL;
  updateList(list, 0, -1, NULL, NULL, NULL);

  if (freeHeadList == NULL) {
    freeHeadList = list;
  } else {
    list->next = freeHeadList;
    freeHeadList = list;
  }
}

/**
 * Finds and returns a free list node
 * @return Returns its reference on success. Returns a NULL pointer on failure.
 */
ListNode* allocateNode() {
  if (freeNodeList != NULL) {
    ListNode *nodeFree = freeNodeList;
    freeNodeList = freeNodeList->next;
    return nodeFree;
  } else {
    return NULL;
  }
}

/**
 * Updates the value of members in a list node
 * @param listNode the list node to be updated
 * @param item the value of list->item after updating
 * @param prev the value of list->prev after updating
 * @param next the value of list->next after updating
 */
void updateListNode(ListNode* listNode, void* item, ListNode* prev, ListNode* next) {
  listNode->val = item;
  listNode->prev = prev;
  listNode->next = next;
}

/**
 * Puts a list node back to free list node pool
 * @param the list node to be free
 */
void reclaimNode(ListNode* listNode) {
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

void ListReversedPrint(LIST *list) {
  ListNode* iter = list->tail;
  while(iter != NULL) {
    printf("%d ", *(int*)iter->val);
    iter = iter->prev;
  }
  printf("\n");
}
