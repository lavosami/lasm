#include "lasm.h"
#include <stdio.h>

void main() {
  HashTable* mnemonics = newHashTable();
  initMnemonicsTable(mnemonics);
  HashTable* names = newHashTable();
  FILE* fp;

  fp = fopen("test.asm", "r");
  if (fp == NULL) {
    printf("Cannot open the file.\n");
    return;
  }
  firstParse(fp, names);
  fclose(fp);

  fp = fopen("test.asm", "r");
  if (fp == NULL) {
    printf("Cannot open the file.\n");
    return;
  }
  secondParse(fp, names, mnemonics);
  fclose(fp);

  printf("\n\n");
  print(names);
  printf("\n\n");
  print(mnemonics);
}
