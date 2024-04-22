#include "lasm.h"
#include "hash_map.h"

// void process(char *str, Lasm *model)
// {
//   getTag(str, model);
//   getComment(str, model);
//   getOperator(str, model);
//   getOperand(str, model);
//   print(*model);

//   // Free allocated memory
//   free(model->tag);
//   free(model->comment);
//   free(model->operator);
//   free(model->operand);
// }

// int main(int argc, char **argv)
// {
//   Lasm model;

//   FILE *file;
//   file = fopen(argv[1], "r");
//   char str[512];

//   while (fgets(str, 512, file))
//   {
//     process(str, &model);
//   }

//   fclose(file);

//   return 0;
// }

int main() {
  HashTable* table = createHashTable(); // Размер таблицы = 17

  insertElement(table, "key1", 10);
  insertElement(table, "key2", 20);
  insertElement(table, "key3", 30); // Возможна коллизия

  int value = findElement(table, "key2");
  printf("Value for key2: %d\n", value); // Выведет: Value for key2: 20

  deleteElement(table, "key1");

  clearHashTable(table);

  return 0;
}
