#ifndef LASM_HEADER
#define LASM_HEADER

#define commentChar ';'

#include <string.h>
#include <stdlib.h>

// НЕ НАЗЫВАЙ ПЕРЕМЕННЫЕ ОДНОЙ БУКВОЙ БЛЯТЬ.

typedef struct Lasm {
  char* tag;
  char* operator;
  char* operand;
  char* comment;
} Lasm;

void getComment(char* str, Lasm* lasm) {
  size_t i;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] == commentChar) {
      // Allocate memory for lasm->comment
      lasm->comment = malloc(strlen(str) - i);
      strncpy(lasm->comment, str + i + 1, strlen(str) - i - 1);
      // Ensure the string is null-terminated
      lasm->comment[strlen(str) - i - 1] = '\0';
      return; // Exit after getting the comment
    }
  }
  // If there's no comment, set it to an empty string
  lasm->comment = strdup("");
}

void getTag(char* str, Lasm* lasm) {
  size_t i;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] == ':') {
      // Allocate memory for lasm->tag
      lasm->tag = malloc(i + 1);
      strncpy(lasm->tag, str, i);
      // Ensure the string is null-terminated
      lasm->tag[i] = '\0';
      return; // Exit after getting the tag
    }
  }
  // If there's no tag, set it to an empty string
  lasm->tag = strdup("");
}

#endif