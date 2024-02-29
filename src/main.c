#include <stdio.h>
#include "lasm.h"

int main(int argc, char** argv) {
  Lasm model;
  char* str = "_start:                             ; Начало программы";
  getTag(str, &model);
  getComment(str, &model);
  printf("%s, %s\n", model.comment, model.tag);

  // Free allocated memory
  free(model.tag);
  free(model.comment);
  
  return 0;
}
