#include <stdio.h>
#include "lasm.h"

int main(int argc, char** argv) {
  Lasm model;
  char* str = "; Программа для демонстрации разбора строк";
  getTag(str, &model);
  getComment(str, &model);
  getOperator(str, &model);
  getOperand(str, &model);
  print(model);

  // Free allocated memory
  free(model.tag);
  free(model.comment);
  free(model.operator);
  free(model.operand);
  
  return 0;
}
