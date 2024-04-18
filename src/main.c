#include "lasm.h"

void process(char* str, Lasm* model) {
  getTag(str, model);
  getComment(str, model);
  getOperator(str, model);
  getOperand(str, model);
  print(*model);

  // Free allocated memory
  free(model->tag);
  free(model->comment);
  free(model->operator);
  free(model->operand);
}


int main(int argc, char** argv) {
  Lasm model;
  //   char* str = "            mov    ah, 9h";
  // getTag(str, &model);
  // getComment(str, &model);
  // getOperator(str, &model);
  // getOperand(str, &model);
  // print(model);

  // // Free allocated memory
  // free(model.tag);
  // free(model.comment);
  // free(model.operator);
  // free(model.operand);
  

  FILE* file;
  file = fopen(argv[1], "r");

  char* str;

  while(fgets(str, 512, file)) {
    process(str, &model);
  }

  fclose(file);

  return 0;
}