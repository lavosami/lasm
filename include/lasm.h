#ifndef LASM_HEADER
#define LASM_HEADER

#define comSymb ';'

#include <string.h>
#include <stdlib.h>

// НЕ НАЗЫВАЙ ПЕРЕМЕННЫЕ ОДНОЙ БУКВОЙ БЛЯТЬ.

typedef struct Lasm {
  char* lable;
  char* oper;
  char* operand;
  char* comment;
} Lasm;

char* substring(char* str, size_t begin, size_t length) {
  size_t it = 0;
  char* result = (char*)malloc(sizeof(char) * length);
  while (it < length) {
    result[it] = str[begin+it-1];
    it++;
  }
  result[it] = '\0';
  return result;
}

char* findComment(char* str) {
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == comSymb) {
      return substring(str, i + 2, strlen(str)-i + 1);
    }
  }
  return "";
}

char* getTag(char* str) {
  int8_t pos = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == ';') {
      pos = -1;
      return "";
    }

    if (str[i] == ":") {
      pos = i;
    }
  }
  
  return substring(str, 0, pos);
}

#endif
