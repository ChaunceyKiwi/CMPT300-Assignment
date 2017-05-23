#include "../include/list.h"

//////////////////////////////////////////////
// Global initialization

LIST headsPool[headsPoolSize];
int freeHeadIndex = 0;
ListNode nodesPool[nodesPollSize];
int freeNodeIndex = 0;
LIST *freeHeadList = NULL;
ListNode *freeNodeList = NULL;

//////////////////////////////////////////////
// Helper functions declaration

void updateListNode(ListNode* listNode, void* item, ListNode* prev, ListNode* next);
void updateList(LIST* list, int len, int currFlag,ListNode* head, ListNode* tail, ListNode* curr);
LIST* allocateList();
ListNode* allocateNode();
void FreeList(LIST* list);
void FreeNode(ListNode* listNode);
void ListConcat(LIST* list1, LIST* list2);

//////////////////////////////////////////////
// Routine functions implementation

LIST *ListCreate() {
  LIST *list= allocateList();
  if (list != NULL) {
    // Initialize the current pointer before the header
    updateList(list, 0, -1, NULL, NULL, NULL);
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

void *ListNext(LIST* list) {
  // If the current pointer points out of the list, return NULL
  // No backtracking
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

void *ListPrev(LIST* list) {
  // If the current pointer points out of the list, return NULL
  // No backtracking
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

void *ListCurr(LIST* list) {
  if (list->curr != NULL) {
    return list->curr->val;
  } else {
    return NULL;
  }
}

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

void ListConcat(LIST* list1, LIST* list2) {
  // Do concatenation only when both of the lists are not NULL
  if (list1 != NULL && list2 != NULL) {
    list1->tail->next = list2->head;
    list2->head->prev = list1->tail;
    FreeList(list2);
  }
}

void ListFree(LIST* list, void *itemFree(LIST* list)) {
  ListFirst(list);
  while(list->curr != NULL) {
    (*itemFree)(list);
  }
  FreeList(list);
}

void *ListTrim(LIST* list) {
  ListLast(list);
  return ListRemove(list);
}

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


//////////////////////////////////////////////
// Comparator routine functions declaration
int intEqualTo(LIST* list, void* comparisonArg) {
  return *(int*)(list->curr->val) == *(int*)(comparisonArg);
}

//////////////////////////////////////////////
// Helper functions implementation

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

//////////////////////////////////////////////
// Testing functions implementation

void ListPrint(LIST *list) {
  ListNode* iter = list->head;
  while(iter != NULL) {
    printf("%d ", *(int*)iter->val);
    iter = iter->next;
  }
  printf("\n");
}
