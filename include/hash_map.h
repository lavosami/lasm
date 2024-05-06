#ifndef HASH_MAP_HEADER
#define HASH_MAP_HEADER

#include "configuration.h"

typedef struct Node {
  unsigned char* key;
  char* value;
  struct Node* next;
} Node;

typedef struct HashTable {
  int m;
  Node** table;
} HashTable;

unsigned int hashFunction(unsigned char* key, int m) {
  unsigned int xor = 0;
  for (int i = 0; i < strlen(key); i++) {
    xor ^= key[i];
  }
  return xor % m;
}

HashTable* createTable(int m) {
  HashTable* table = malloc(sizeof(HashTable));
  if (table == NULL) {
    return NULL;
  }

  table->m = m;
  table->table = malloc(m * sizeof(Node *));
  if (table->table == NULL) {
    free(table);
    return NULL;
  }

  for (int i = 0; i < m; i++) {
    table->table[i] = NULL;
  }

  return table;
}

void insert(HashTable* table, unsigned char *key, char *value) {
  int hash_index = hashFunction(key, table->m);

  Node* new_node = malloc(sizeof(Node));
  if (new_node == NULL) {
    return;
  }

  new_node->key = malloc(strlen(key));
  if (new_node->key == NULL) {
    free(new_node);
    return;
  }

  memcpy(new_node->key, key, strlen(key));
  new_node->value = value;
  new_node->next = table->table[hash_index];

  table->table[hash_index] = new_node;
}

char* search(HashTable* table, unsigned char* key) {
  int hash_index = hashFunction(key, table->m);

  Node* current_node = table->table[hash_index];
  while (current_node != NULL) {
    if (memcmp(current_node->key, key, strlen(key)) == 0) {
      return current_node->value;
    }
    current_node = current_node->next;
  }

  return NULL;
}

void delete(HashTable* table, unsigned char* key) {
  int hash_index = hashFunction(key, table->m);

  Node* previous_node = NULL;
  Node* current_node = table->table[hash_index];

  while (current_node != NULL) {
    if (memcmp(current_node->key, key, strlen(key)) == 0) {
      // deletion from the start of chain
      if (previous_node == NULL) {
        table->table[hash_index] = current_node->next;
      } else {
        previous_node->next = current_node->next;
      }
      free(current_node->key);
      free(current_node);
      return;
    }

    previous_node = current_node;
    current_node = current_node->next;
  }
}

void deleteTable(HashTable *table) {
  if (table == NULL) return;

  for (int i = 0; i < table->m; i++) {
    Node* current_node = table->table[i];
    while (current_node != NULL) {
      Node* next_node = current_node->next;
      free(current_node->key);
      free(current_node);
      current_node = next_node;
    }
  }

  free(table->table);
  free(table);
}


#endif
