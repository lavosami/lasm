#ifndef HASH_MAP_HEADER
#define HASH_MAP_HEADER

#include "configuration.h"

typedef struct Element {
  char*           key;
  int             value;
  struct Element* next;
} Element;

typedef struct HashTable {
  int       size;
  Element** table;
  int       elementsCount;
  int       (*hashFunction)(char* key);
} HashTable;


int hashFunction(char* key) {
  int hash = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    hash = (hash ^ key[i]) % KEYS_COUNT;  // xor combining multibyte keys
  }
  return hash;
}

HashTable* createHashTable() {
  HashTable* table = (HashTable*)(sizeof(HashTable));
  table->size = KEYS_COUNT;
  table->elementsCount = 0;
  table->table = (Element**)(KEYS_COUNT * sizeof(Element*));

  for (int i = 0; i < KEYS_COUNT; i++) {
    table->table[i] = NULL;
  }

  table->hashFunction = hashFunction; // hash function setting
  return table;
}

void insertElement(HashTable* table, char* key, int value) {
  int index = table->hashFunction(key);

  Element* newElement = (Element*)(sizeof(Element));
  newElement->key = (char*)malloc(strlen(key) + 1);
  strcpy(newElement->key, key);
  newElement->value = value;
  newElement->next = NULL;

  // handling collisions using the chain method
  if (table->table[index] != NULL) {
    newElement->next = table->table[index];
    table->table[index] = newElement;
  } else {
    table->table[index] = newElement;
  }

  table->elementsCount++;
}

int findElement(HashTable* table, char* key) {
  int index = table->hashFunction(key);
  Element* current = table->table[index];

  while (current != NULL) {
    if (strcmp(current->key, key) == 0) return current->value;
    current = current->next;
  }

  return -1;
}

void deleteElement(HashTable* table, char* key) {
  int index = table->hashFunction(key);
  Element** prev = &table->table[index];
  Element* current = table->table[index];

  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      *prev = current->next;
      free(current->key);
      free(current);
      table->elementsCount--;
      return;
    }
    prev = &current->next;
    current = current->next;
  }
}

void clearHashTable(HashTable* table) {
  for (int i = 0; i < table->size; i++) {
    Element* current = table->table[i];
    while (current != NULL) {
      Element* next = current->next;
      free(current->key);
      free(current);
      current = next;
    }
    table->table[i] = NULL;
  }
  free(table->table);
  free(table);
}

#endif
