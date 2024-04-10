#include <stdio.h>
#include "lasm.h"

int main(int argc, char** argv) {
  Lasm model;
  char* str = "    message: DB 'Hello,World!'     ; Объявление строки";
  getTag(str, &model);
  getComment(str, &model);
  getOperator(str, &model);
  getOperand(str, &model);
  print(model);

  // Free allocated memory
  free(model.tag);
  free(model.comment);
  free(model.operator);
  
  return 0;
}
