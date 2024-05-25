#include "configuration.h"

typedef struct {
  int hashKey;
  char* key;
  int mark;
  char* info;
} DataItem;

typedef struct {
  DataItem* hashArray[TABLE_SIZE];
  int originalHashIndexes[TABLE_SIZE];
  int indexOHI;
  int itemsCount;
  int maxHashIndex;
  int collisions;
} HashTable;

HashTable* newHashTable() {
  HashTable* table = (HashTable*)malloc(sizeof(HashTable));

  for (int i = 0; i < TABLE_SIZE; i++) {
    DataItem* newItem = (DataItem*)malloc(sizeof(DataItem));
    newItem->key = " ";
    newItem->mark = -1;
    newItem->info = " ";
    table->hashArray[i] = newItem;
  }

  table->indexOHI = 0;
  table->itemsCount = 0;
  table->maxHashIndex = 0;
  table->collisions = 0;
  return table;
}

int hashCode(char* key, HashTable* table) {
  int hash = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    hash = hash + key[i];
  }
  hash %= 127;
  if (table != NULL) {
    table->originalHashIndexes[table->indexOHI] = hash;
    table->indexOHI++;
  }
  return hash;
}

DataItem* search(char* key, HashTable* table) {
  int hashIndex;
  int selfIndex;
  hashIndex = hashCode(key, NULL);
  selfIndex = hashIndex - 1;
  bool firststep = true;
  while (table->hashArray[hashIndex] != NULL && selfIndex != hashIndex) {
    if (firststep) {
      firststep = false;
      selfIndex++;
    }
    if (hashIndex < 0) {
      return NULL;
    }
    if (strcmp(table->hashArray[hashIndex]->key, key) == 0) {
      return table->hashArray[hashIndex];
    }
    if (hashIndex == TABLE_SIZE - 1) {
      hashIndex = 0;
    } else {
      ++hashIndex;
    }
  }

  return NULL;
}

void add(char* key, char* info, int mark, HashTable* table) {
  DataItem* item = (DataItem*)malloc(sizeof(DataItem));
  int hashIndex;
  item->key = key;
  hashIndex = hashCode(key, table);
  int selfIndex = hashIndex - 1;
  bool firststep = true;
  bool hasCollision = false;

  if (hashIndex < 0) {
    return;
  }

  while (table->hashArray[hashIndex] != NULL &&
         table->hashArray[hashIndex]->key != " " &&
         strcmp(table->hashArray[hashIndex]->key, key) != 0 &&
         selfIndex != hashIndex) {
    if (firststep) {
      firststep = false;
      selfIndex++;
    }
    if (!hasCollision) {
      table->collisions++;
      hasCollision = true;
    }
    if (hashIndex == TABLE_SIZE - 1) {
      hashIndex = 0;
    } else {
      ++hashIndex;
    }
  }

  if (selfIndex == hashIndex) {
    return;
  }
  item->mark = mark;
  item->info = info;
  item->hashKey = hashIndex;
  if (hashIndex > table->maxHashIndex) {
    table->maxHashIndex = hashIndex;
  }
  table->hashArray[hashIndex] = item;
  table->itemsCount++;
}

void print(HashTable* table) {
  int i;

  for (i = 0; i <= table->maxHashIndex; i++) {
    if (table->hashArray[i]->mark != -1) {
      printf(" (%8s,%4d,%4d", table->hashArray[i]->key,
             table->hashArray[i]->hashKey, table->hashArray[i]->mark);
      if (table->hashArray[i]->mark == 0 || table->hashArray[i]->mark == 1) {
        printf(",%4s", table->hashArray[i]->info);
      }
      printf(")\n");
    } else {
    }
  }
  printf("\n");
}