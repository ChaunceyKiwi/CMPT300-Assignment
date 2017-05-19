#include "list.h"

LIST headsPool[headsPoolSize];
int freeHeadIndex = 0;
ListNode nodesPool[nodesPollSize];
int freeNodeIndex = 0;

//////////////////////////////////////////////
// helper functions

void updateListNode(ListNode* listNode, void* item, ListNode* prev, ListNode* next) {
  listNode->val = item;
  listNode->prev = prev;
  listNode->next = next;
}

void updateList(LIST* list, int len, ListNode* head, ListNode* tail, ListNode* curr) {
  list->len = len;
  list->head = head;
  list->tail = tail;
  list->curr = curr;
}

//////////////////////////////////////////////
// routine functions

LIST *ListCreate() {
  LIST *list= &headsPool[freeHeadIndex++];
  updateList(list, 0, NULL, NULL, NULL);
  return list;
}

int ListCount(LIST* list) {
  return list->len;
}

void *ListFirst(LIST* list) {
  list->curr = list->head;
  return list->curr->val;
}

void *ListLast(LIST* list) {
  list->curr = list->tail;
  return list->curr->val;
}

void *ListNext(LIST* list) {
  // Advance list's current item by one
  list->curr = list->curr->next;
  
  // If not beyond the end of the list, return the item, otherwise NULL
  if (list->curr != NULL) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

void *ListPrev(LIST* list) {
  // Back up list's current item by one
  list->curr = list->curr->prev;
  
  // If not beyond the start of the list, return the item, otherwise NULL
  if (list->curr != NULL) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

void *ListCurr(LIST* list) {
  // If the current pointer is before or beyond the list, return NULL
  if (list->curr != NULL) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

// adds the new item to list directly after the current
// item, and makes item the current item.
int ListAdd(LIST* list, void* item) {
  
  // Check if current pointer is NULL
  if (list->curr == NULL) {
    return -1;
  }
  
  // Check if nodes are exhausted
  else if (list->len >= nodesPollSize) {
    return -1;
  }
  
  else {
    ListNode* temp = list->curr;
    list->curr = &nodesPool[freeNodeIndex++];
    updateListNode(list->curr, item, temp, temp->next);
    
    // If current pointer is at the tail
    if (temp->next != NULL) {
      temp->next->prev = list->curr;
    } else {
      list->tail = list->curr;
    }
    
    temp->next = list->curr;
    list->len++;
    return 0;
  }
}

// adds item to list directly before the current item,
// and makes the new item the current one.
int ListInsert(LIST* list, void* item) {
  
  // Check if current pointer is NULL
  if (list->curr == NULL) {
    return -1;
  }
  
  // Check if nodes are exhausted
  else if (list->len >= nodesPollSize) {
    return -1;
  }
  
  else {
    ListNode* temp = list->curr;
    list->curr = &nodesPool[freeNodeIndex++];
    updateListNode(list->curr, item, temp, temp->prev);
    
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

// adds item to the end of list, and makes the new item the current one.
int ListAppend(LIST* list, void* item) {
  
  // Check if nodes are exhausted
  if (list->len >= nodesPollSize) {
    return -1;
  }
  
  // If the list is empty
  if (list->len == 0) {
    list->curr = &nodesPool[freeNodeIndex++];
    updateListNode(list->curr, item, NULL, NULL);
    updateList(list, list->len + 1, list->curr, list->curr, list->curr);
    return 0;
  }
  
  // If the list is not empty
  else {
    list->curr = &nodesPool[freeNodeIndex++];
    updateListNode(list->curr, item, list->tail, NULL);
    
    list->tail->next = list->curr;
    list->tail = list->curr;
    list->len++;
    
    return 0;
  }
  
  return -1;
}

// adds item to the front of list, and makes the new item the current one.
int ListPrepend(LIST* list, void* item) {
  
  // Check if nodes are exhausted
  if (list->len >= nodesPollSize) {
    return -1;
  }
  
  // If the list is empty
  if (list->len == 0) {
    list->curr = &nodesPool[freeNodeIndex++];
    
    updateListNode(list->curr, item, NULL, NULL);
    updateList(list, list->len + 1, list->curr, list->curr, list->curr);
    return 0;
  }
  
  // If the list is not empty
  else {
    list->curr = &nodesPool[freeNodeIndex++];
    updateListNode(list->curr, item, NULL, list->head);
    
    list->head->prev = list->curr;
    list->head = list->curr;
    list->len++;
    
    return 0;
  }
  
  return -1;
}
