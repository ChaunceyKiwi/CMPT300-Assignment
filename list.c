#include "list.h"
#include "stack.h"

LIST headsPool[headsPoolSize];
int freeHeadIndex = 0;
ListNode nodesPool[nodesPollSize];
int freeNodeIndex = 0;
STACK freeHeadsStack = {.topIndex = -1};
STACK freeNodesStack = {.topIndex = -1};

void updateListNode(ListNode* listNode, void* item, ListNode* prev, ListNode* next);
void updateList(LIST* list, int len, int currFlag,ListNode* head, ListNode* tail, ListNode* curr);
LIST* allocateList();
ListNode* allocateNode();

//////////////////////////////////////////////
// routine functions

LIST *ListCreate() {
  LIST *list= allocateList();
  if (list != NULL) {
    updateList(list, 0, 0, NULL, NULL, NULL);
  }
  return list;
}

int ListCount(LIST* list) {
  return list->len;
}

void *ListFirst(LIST* list) {
  list->curr = list->head;
  return ListCurr(list);
}

void *ListLast(LIST* list) {
  list->curr = list->tail;
  return ListCurr(list);
}

// Advance list's current item by one
void *ListNext(LIST* list) {
  list->curr = list->curr->next;
  void *res = ListCurr(list);
  
  // set flag when the current pointer goes beyond the tail
  if (res == NULL) {
    list->currFlag = 1;
  }
  
  return res;
}

// Back up list's current item by one
void *ListPrev(LIST* list) {
  list->curr = list->curr->prev;
  
  void *res = ListCurr(list);
  
  // set flag when the current pointer goes before the head
  if (res == NULL) {
    list->currFlag = -1;
  }
  
  return res;
}

void *ListCurr(LIST* list) {
  if (list->curr != NULL) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

int ListAdd(LIST* list, void* item) {
  ListNode* temp = list->curr;
  list->curr = allocateNode();

  // Fail cases:
  // 1. Current pointer is NULL
  // 2. No more free nodes in nodes pool
  // 3. The maximum list size is reached
  if (temp == NULL || list->curr == NULL || list->len >= nodesPollSize) {
    return -1;
  }
  
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

int ListInsert(LIST* list, void* item) {
  ListNode* temp = list->curr;
  list->curr = allocateNode();
  
  // Fail cases:
  // 1. Current pointer is NULL
  // 2. No more free nodes in nodes pool
  // 3. The maximum list size is reached
  if (temp == NULL || list->curr == NULL || list->len >= nodesPollSize) {
    return -1;
  }
  
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

// adds item to the end of list, and makes the new item the current one.
int ListAppend(LIST* list, void* item) {
  
  // Check if nodes are exhausted
  if (list->len >= nodesPollSize) {
    return -1;
  }
  
  // If the list is empty
  if (list->len == 0) {
    list->curr = allocateNode();
    updateListNode(list->curr, item, NULL, NULL);
    updateList(list, list->len + 1, 0, list->curr, list->curr, list->curr);
    return 0;
  }
  
  // If the list is not empty
  else {
    list->curr = allocateNode();
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
    list->curr = allocateNode();
    
    updateListNode(list->curr, item, NULL, NULL);
    updateList(list, list->len + 1, 0, list->curr, list->curr, list->curr);
    return 0;
  }
  
  // If the list is not empty
  else {
    list->curr = allocateNode();
    updateListNode(list->curr, item, NULL, list->head);
    
    list->head->prev = list->curr;
    list->head = list->curr;
    list->len++;
    
    return 0;
  }
  
  return -1;
}

//////////////////////////////////////////////
// helper functions

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
  if (isEmpty(&freeHeadsStack)) {
    if (freeHeadIndex < headsPoolSize) {
      return &headsPool[freeHeadIndex++];
    } else {
      return NULL;
    }
  } else {
    return &headsPool[pop(&freeHeadsStack)];
  }
}

ListNode* allocateNode() {
  if (isEmpty(&freeNodesStack)) {
    if (freeNodeIndex < nodesPollSize) {
      return &nodesPool[freeNodeIndex++];
    } else {
      return NULL;
    }
  } else {
    return &nodesPool[pop(&freeNodesStack)];
  }
}
