#include "list.h"

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
    list->curr->val = item;
    list->curr->prev = temp;
    list->curr->next = temp->next;
    
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
    list->curr->val = item;
    list->curr->next = temp;
    list->curr->prev = temp->prev;
    
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
    list->curr->val = item;
    list->curr->prev = NULL;
    list->curr->next = NULL;
    list->head = list->curr;
    list->tail = list->curr;
    list->len++;
    return 0;
  }
  
  // If the list is not empty
  else {
    list->curr = &nodesPool[freeNodeIndex++];
    list->curr->val = item;
    list->curr->prev = list->tail;
    list->curr->next = NULL;
    
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
    list->curr->val = item;
    list->curr->prev = NULL;
    list->curr->next = NULL;
    list->head = list->curr;
    list->tail = list->curr;
    list->len++;
    return 0;
  }
  
  // If the list is not empty
  else {
    list->curr = &nodesPool[freeNodeIndex++];
    list->curr->val = item;
    list->curr->prev = NULL;
    list->curr->next = list->head;
    
    list->head->prev = list->curr;
    list->head = list->curr;
    
    list->len++;
    
    return 0;
  }
  
  return -1;
}
