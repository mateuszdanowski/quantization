#include "list.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

ListElement makeNewListElem(void *ptr) {
  ListElement elem;
  elem.ptr = ptr;
  return elem;
}

ListNode *makeNewListNode(ListNode *prev, ListElement elem, ListNode *next) {
  ListNode *node = (ListNode *)malloc(sizeof(ListNode));
  if (node == NULL) {
    exit(1);
  }

  node->prev = prev;
  node->elem = elem;
  node->next = next;
  return node;
}

List *makeNewList(uint64_t newEnergy) {
  List *list = (List *)malloc(sizeof(List));
  if (list == NULL) {
    exit(1);
  }

  list->head = makeNewListNode(NULL, makeNewListElem(list), NULL);
  list->tail = makeNewListNode(list->head, makeNewListElem(list), NULL);

  list->head->next = list->tail;

  list->energy = newEnergy;
  list->numOfElements = 0;
  return list;
}

void setEnergy(ListNode *node, uint64_t newEnergy) {
  List *listPtr = node->elem.ptr;
  listPtr->energy = newEnergy;
}

ListNode *pushBack(List *list) {
  ListElement elem = list->head->elem;
  ListNode *back = list->tail->prev;

  ListNode *oldNext = list->tail->prev->next;
  ListNode *newNode = makeNewListNode(back, elem, oldNext);

  back->next = newNode;
  oldNext->prev = newNode;

  list->numOfElements++;
  return list->tail->prev;
}

void deleteNode(ListNode *node) {
  node->next->prev = node->prev;
  node->prev->next = node->next;

  List *list = node->elem.ptr;

  list->numOfElements--;
  free(node);

  if (list->numOfElements == 0) {
    free(list->head);
    free(list->tail);
    free(list);
  }
}

void pushList(List *list1, List *list2) {
  list1->numOfElements += list2->numOfElements;

  uint64_t r1 = (list1->energy) % 2;
  uint64_t r2 = (list2->energy) % 2;

  list1->energy = list1->energy / 2 + list2->energy / 2 + (r1 + r2 == 2);

  ListNode *iter = list2->head->next;
  ListNode *end = list2->tail;

  while (iter != end) {
    iter->elem.ptr = list1;
    iter = iter->next;
  }

  list2->head->next->prev = list1->tail->prev;
  list1->tail->prev->next = list2->head->next;

  list2->tail->prev->next = list1->tail;
  list1->tail->prev = list2->tail->prev;

  free(list2->head);
  free(list2->tail);
  free(list2);
}

void mergeLists(List *list1, List *list2) {
  if (list1->numOfElements > list2->numOfElements) {
    pushList(list1, list2);
  } else {
    pushList(list2, list1);
  }
}
