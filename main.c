#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "trie.h"

#define INITIAL_LINE_LENGTH 32

// Wywoływana na początek programu, alokuje potrzebną pamięć
void initialize(char **line, size_t *lineLength) {
  *lineLength = INITIAL_LINE_LENGTH;
  *line = malloc(*lineLength * sizeof(char));

  // Jeśli malloc się nie udał
  if (*line == NULL) {
    exit(1);
  }
  root = getNewTrieNode();
}

// Wywoływana przed zakończeniem programu, zwalnia całą pamięć
void clean(char **line, size_t *lineLength) {
  free(*line);
  *lineLength = 0;

  // Wyczyść całe drzewo
  deleteTrie(&root);
}

// Sprawdza, czy podany ciąg znaków jest dopuszczalny
int isCorrectHistory(char *str) {
  if (str == NULL) {
    return 0;
  }

  while (*str) {
    if (*str < '0' || '3' < *str) {
      return 0;
    }
    str++;
  }
  return 1;
}

// Sprawdza, czy podany ciąg znaków jest liczbą nieujemną
int isCorrectEnergy(char *str) {
  if (str == NULL) {
    return 0;
  }

  while (*str) {
    if (*str < '0' || '9' < *str) {
      return 0;
    }
    str++;
  }
  return 1;
}

// Zamienia ciąg znaków reprezentujący energię w liczbę i ją zwraca;
// jeśli nie jest on poprawny zwraca 0
uint64_t getEnergy(char *str) {
  if (str == NULL) {
    return 0;
  }
  if (!isCorrectEnergy(str)) {
    return 0;
  }

  uint64_t energy = strtoull(str, NULL, 10);
  if (errno == ERANGE || energy == 0) {
    errno = 0;
    return 0;
  }
  return energy;
}

// Wczytuje linię ze standardowego wejścia
int readLine(char **line, size_t *lineLength) {
  int character;
  unsigned int pos = 0;

  while ((character = fgetc(stdin)) != '\n' && character != EOF) {
    if (pos + 1 >= *lineLength) {
      *lineLength *= 2;
      *line = realloc(*line, *lineLength);

      // Jeśli realloc się nie udał
      if (*line == NULL) {
        exit(1);
      }
    }

    // Jeśli na wejściu pojawi się '\0', zmieniam go na inny niepoprawny
    // znak po to, aby nie sygnalizował on końca stringa
    if (character == '\0') {
      character = '!';
    }

    (*line)[pos++] = (char)character;
  }

  // zastępuję ostatni znak w linii znakiem końca stringa
  (*line)[pos] = '\0';

  // Jeśli wiersz nie kończy się znakiem '\n', a nie jest wierszem pustym
  if (character == EOF && pos != 0) {
    fprintf(stderr, "ERROR\n");
  }

  return character != EOF;
}

// Wykonuje operacje dla danej linii
void processLine(char *line) {
  size_t numOfCharacters = strlen(line) + 1;  // + 1 bo z '\0'

  // komentarze lub pusty wiersz
  if (numOfCharacters == 1 || line[0] == '#') {
    return;
  }

  char *command = strtok(line, " \n");  // typ operacji
  char *arg1 = strtok(NULL, " \n");     // pierwszy argument
  char *arg2 = strtok(NULL, " \n");     // drugi argument
  char *rest = strtok(NULL, " \n");     // jeśli jest coś jeszcze

  size_t commandLength = 0, arg1Length = 0, arg2Length = 0;

  if (command != NULL) {
    commandLength = strlen(command) + 1;
  }
  if (arg1 != NULL) {
    arg1Length = strlen(arg1) + 1;
  }
  if (arg2 != NULL) {
    arg2Length = strlen(arg2) + 1;
  }

  size_t expectedLength = commandLength + arg1Length + arg2Length;

  if (rest != NULL || numOfCharacters != expectedLength) {
    fprintf(stderr, "ERROR\n");
    return;
  }

  int flag = 1;
  if (strcmp(command, "DECLARE") == 0) {
    flag = !(isCorrectHistory(arg1) && arg2 == NULL);
    if (!flag) {
      insertStr(arg1);
    }
  } else if (strcmp(command, "REMOVE") == 0) {
    flag = !(isCorrectHistory(arg1) && arg2 == NULL);
    if (!flag) {
      removeStr(arg1);
    }
  } else if (strcmp(command, "VALID") == 0) {
    flag = !(isCorrectHistory(arg1) && arg2 == NULL);
    if (!flag) {
      int isValid = searchStr(arg1);
      if (isValid) {
        fprintf(stdout, "YES\n");
      } else {
        fprintf(stdout, "NO\n");
      }
      return;
    }
  } else if (strcmp(command, "ENERGY") == 0) {
    if (arg2 != NULL) {
      uint64_t energy = getEnergy(arg2);

      flag = !(isCorrectHistory(arg1) && searchStr(arg1) && energy > 0);
      if (!flag) {
        insertEnergy(arg1, energy);
      }
    } else if (arg1 != NULL) {
      flag = !(isCorrectHistory(arg1) && searchStr(arg1));
      if (!flag) {
        uint64_t energy = getEnergyStr(arg1);
        if (energy == 0) {
          flag = 1;
        }
        if (!flag) {
          fprintf(stdout, "%" PRIu64 "\n", energy);
          return;
        }
      }
    }
  } else if (strcmp(command, "EQUAL") == 0) {
    flag = !(isCorrectHistory(arg1) && isCorrectHistory(arg2) &&
             searchStr(arg1) && searchStr(arg2));
    if (!flag) {
      uint64_t energy1 = getEnergyStr(arg1);
      uint64_t energy2 = getEnergyStr(arg2);

      if (energy1 == 0 && energy2 == 0 && strcmp(arg1, arg2) != 0) {
        flag = 1;
      }
      if (!flag && strcmp(arg1, arg2) != 0) {
        makeEqual(arg1, arg2);
      }
    }
  }

  if (flag) {
    fprintf(stderr, "ERROR\n");
  } else {
    fprintf(stdout, "OK\n");
  }
}

int main() {
  size_t lineLength;
  char *line;

  initialize(&line, &lineLength);
  while (readLine(&line, &lineLength)) {
    processLine(line);
  }

  clean(&line, &lineLength);
  return 0;
}
