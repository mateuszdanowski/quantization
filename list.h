#ifndef LIST_H
#define LIST_H

#include <inttypes.h>
#include <stdlib.h>

// Elementy przechowywane w wierzchołku listy
typedef struct ListElement {
  void *ptr;
} ListElement;

// Wierzchołek listy z przechowywanym elementem
// i wskaźnikami na poprzedni i następny wierzchołek
typedef struct ListNode {
  ListElement elem;
  struct ListNode *prev;
  struct ListNode *next;
} ListNode;

// Lista, reprezentowana jako wskaźnik na głowę i ogon, czyli wierzchołki,
// które są strażnikami; oprócz tego przechowuje jej rozmiar oraz energię,
// jaką posiadają jej elementy
typedef struct List {
  ListNode *head;
  ListNode *tail;
  uint64_t energy;
  size_t numOfElements;
} List;

// Tworzy nowy element listy
ListElement makeNewListElem(void *ptr);

// Tworzy nowy wierzchołek listy
ListNode *makeNewListNode(ListNode *prev, ListElement elem, ListNode *next);

// Tworzy nową lis†ę
List *makeNewList(uint64_t newEnergy);

// Przypisuje energię liście, w której jest dany wierzchołek
void setEnergy(ListNode *node, uint64_t newEnergy);

// Dodaje nowy wierzchołek do danej listy i zwraca do niego wskaźnik
ListNode *pushBack(List *list);

// Usuwa wierzchołek ze swojej listy;
// jeśli był on jedynym elementem tej listy, to ją usuwa
void deleteNode(ListNode *node);

// Przepisuje wszystkie elementy z list2 do list1
void pushList(List *list1, List *list2);

// Rozstrzyga którą listę przepiąć do której, na podstawie ich wielkości
void mergeLists(List *list1, List *list2);

#endif  // LIST_H
