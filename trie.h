#ifndef TRIE_H
#define TRIE_H

#include "list.h"

#define CHAR_SIZE 4

// Wierzchołek drzewa Trie, ze wskaźnikami na potomków
// i wskaźnikiem na element jakim jest w swojej liście
typedef struct Trie {
  ListNode *listNode;
  struct Trie *character[CHAR_SIZE];
} Trie;

// Wskaźnik na korzeń drzewa
Trie *root;

// Zwraca nowy wierzchołek trie
Trie *getNewTrieNode();

// Usuwa całe drzewo z korzeniem w *curr
void deleteTrie(Trie **curr);

// Wstawia ciąg znaków do drzewa Trie
void insertStr(char *str);

// Sprawdza czy podany ciąg znaków istnieje w drzewie
// Zwraca 1 jeśli taki istnieje, wpp 0
int searchStr(char *str);

// Wyszukuje wierzchołek odpowiadający podanemu ciągowi i usuwa jego poddrzewo
void removeStr(char *str);

// Wstawia energie do ciągu, jeśli taki istnieje
void insertEnergy(char *str, uint64_t energy);

// Zwraca energie danego ciągu
// Zwraca 1 jeśli ciąg nie jest dopuszczalny lub nie ma energii
uint64_t getEnergyStr(char *str);

// Zwraca wskaźnik do wierzchołka w drzewie, odpowiadający podanemu ciągowi
// Jeśli ciąg nie jest dopuszczalny zwraca NULL
Trie **getTrieNode(char *str);

// Zwraca wskaźnik do listy,
// w której jest wierzchołek odpowiadający podanemu ciągowi
// Dany wierzchołek musi należeć do jakiejś listy
List *getList(char *str);

// Zrównuje energie dwóch ciągów
void makeEqual(char *str1, char *str2);

#endif  // TRIE_H
