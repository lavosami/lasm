#ifndef HASH_MAP_HEADER
#define HASH_MAP_HEADER

#include "configuration.h"

typedef struct Node {
  unsigned char *key;       // Динамический массив для хранения ключа произвольной длины
  char *value;           // Указатель на значение
  struct Node *next;     // Ссылка на следующий элемент в цепочке
} Node;

typedef struct ChainedHashTable {
  int m;                 // Размер хеш-таблицы
  Node **table;           // Массив указателей на узлы
} ChainedHashTable;

// Хеш-функция, использующая деление и XOR
unsigned int hash_function(unsigned char *key, int m) {
  unsigned int xor = 0;
  for (int i = 0; i < strlen(key); i++) {
    xor ^= key[i];
  }
  return xor % m;
}

// Инициализация хеш-таблицы
ChainedHashTable *create_hash_table(int m) {
  ChainedHashTable *table = malloc(sizeof(ChainedHashTable));
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

// Вставка элемента в хеш-таблицу
void insert_item(ChainedHashTable *table, unsigned char *key, char *value) {
  int hash_index = hash_function(key, table->m);

  Node *new_node = malloc(sizeof(Node));
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

// Поиск элемента в хеш-таблице
char* search_item(ChainedHashTable *table, unsigned char *key) {
  int hash_index = hash_function(key, table->m);

  Node* current_node = table->table[hash_index];
  while (current_node != NULL) {
    if (memcmp(current_node->key, key, strlen(key)) == 0) {
      return current_node->value;
    }
    current_node = current_node->next;
  }

  return NULL;
}

// Удаление элемента из хеш-таблицы
void delete_item(ChainedHashTable *table, unsigned char *key) {
  int hash_index = hash_function(key, table->m);

  Node *previous_node = NULL;
  Node *current_node = table->table[hash_index];

  while (current_node != NULL) {
    if (memcmp(current_node->key, key, strlen(key)) == 0) {
      // Удаление из начала цепочки
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

// Очистка хеш-таблицы
void destroy_hash_table(ChainedHashTable *table) {
  if (table == NULL) return;

  for (int i = 0; i < table->m; i++) {
    Node *current_node = table->table[i];
    while (current_node != NULL) {
      Node *next_node = current_node->next;
      free(current_node->key);
      free(current_node);
      current_node = next_node;
    }
  }

  free(table->table);
  free(table);
}



#endif
