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
  ChainedHashTable* table = create_hash_table(KEYS_COUNT);

  insert_item(table, "key1", "shit");
  insert_item(table, "key2", "mov");
  insert_item(table, "key3", "mov");

  char* value = search_item(table, "key2");
  printf("Value for key2: %s\n", value);
  delete_item(table, "key1");

  destroy_hash_table(table);

  return 0;
}
