#include "trie.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

Trie *getNewTrieNode() {
  Trie *node = (Trie *)malloc(sizeof(Trie));
  if (node == NULL) {
    exit(1);
  }

  node->listNode = NULL;
  for (int i = 0; i < CHAR_SIZE; i++) {
    node->character[i] = NULL;
  }

  return node;
}

void deleteTrie(Trie **curr) {
  if (*curr == NULL) {
    return;
  }

  for (int i = 0; i < CHAR_SIZE; i++) {
    if ((*curr)->character[i] != NULL) {
      deleteTrie(&(*curr)->character[i]);
    }
  }

  if ((*curr)->listNode != NULL) {
    deleteNode((*curr)->listNode);
    (*curr)->listNode = NULL;
  }

  free(*curr);
  (*curr) = NULL;
}

void insertStr(char *str) {
  Trie *curr = root;

  while (*str) {
    if (curr->character[*str - '0'] == NULL) {
      curr->character[*str - '0'] = getNewTrieNode();
    }
    curr = curr->character[*str - '0'];
    str++;
  }
}

int searchStr(char *str) {
  if (root == NULL) {
    return 0;
  }

  Trie *curr = root;
  while (*str) {
    curr = curr->character[*str - '0'];
    if (curr == NULL) {
      return 0;
    }
    str++;
  }
  return 1;
}

void removeStr(char *str) {
  Trie **curr = &root;
  if (*curr == NULL) {
    return;
  }

  while (*str) {
    if ((*curr)->character[*str - '0'] == NULL) {
      return;
    }
    curr = &((*curr)->character[*str - '0']);
    str++;
  }
  deleteTrie(curr);
}

void insertEnergy(char *str, uint64_t energy) {
  Trie *curr = root;

  while (*str) {
    if (curr->character[*str - '0'] == NULL) {
      return;
    }
    curr = curr->character[*str - '0'];
    str++;
  }

  if (curr->listNode == NULL) {
    List *list = makeNewList(energy);
    curr->listNode = pushBack(list);
  }

  setEnergy(curr->listNode, energy);
}

uint64_t getEnergyStr(char *str) {
  Trie *curr = root;

  while (*str) {
    if (curr->character[*str - '0'] == NULL) {
      return 0;
    }
    curr = curr->character[*str - '0'];
    str++;
  }

  if (curr->listNode == NULL) {
    return 0;
  }

  List *listPtr = curr->listNode->elem.ptr;
  return listPtr->energy;
}

Trie **getTrieNode(char *str) {
  Trie **curr = &root;

  while (*str) {
    if ((*curr)->character[*str - '0'] == NULL) {
      return NULL;
    }
    curr = &(*curr)->character[*str - '0'];
    str++;
  }
  return curr;
}

List *getList(char *str) {
  Trie *curr = root;

  while (*str) {
    curr = curr->character[*str - '0'];
    str++;
  }

  List *listPtr = curr->listNode->elem.ptr;
  return listPtr;
}

void makeEqual(char *str1, char *str2) {
  uint64_t energy1 = getEnergyStr(str1);
  uint64_t energy2 = getEnergyStr(str2);

  if (energy1 > 0 && energy2 > 0) {
    List *list1 = getList(str1);
    List *list2 = getList(str2);

    if (list1 != list2) {
      mergeLists(list1, list2);
    }
  } else if (energy1 > 0 && energy2 == 0) {
    List *list = getList(str1);
    Trie **trieNode = getTrieNode(str2);
    (*trieNode)->listNode = pushBack(list);
  } else if (energy1 == 0 && energy2 > 0) {
    List *list = getList(str2);
    Trie **trieNode = getTrieNode(str1);
    (*trieNode)->listNode = pushBack(list);
  }
}
