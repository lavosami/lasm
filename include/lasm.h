#ifndef LASM_HEADER
#define LASM_HEADER

#define MAX_WORDS 128
#define COMMENT_CHAR ';'

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Lasm {
  char* tag;
  char* operator;
  char* operand;
  char* comment;
} Lasm;

char** split(char* input) {
  // allocate memory for strings array
  char** words = (char**)malloc(MAX_WORDS * sizeof(char*));
  if (!words) {
    fprintf(stderr, "%s", "Memory allocation error.");
    exit(EXIT_FAILURE);
  }

  // Create a copy of the input string
  char* temp_input = strdup(input);
  if (!temp_input) {
    fprintf(stderr, "%s", "Memory allocation error.");
    exit(EXIT_FAILURE);
  }

  // go word by word in line
  char* token = strtok(temp_input, " \t\n\r");
  int count = 0;
  while (token != NULL && count < MAX_WORDS) {
    // allocate memory for the current word
    words[count] = (char*)malloc((strlen(token) + 1) * sizeof(char));
    if (!words[count]) {
      fprintf(stderr, "Memory allocation error.");
      exit(EXIT_FAILURE);
    }
    // copy for in count'th element of array words
    strcpy(words[count], token);
    // go to the next word
    token = strtok(NULL, " \t\n\r");
    count++;
  }

  // Free the temporary input string
  free(temp_input);

  return words;
}


void getComment(char* str, Lasm* model) {
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == COMMENT_CHAR) {
      // Allocate memory for model->comment
      model->comment = malloc(strlen(str) - i);
      strncpy(model->comment, str + i + 1, strlen(str) - i - 1);
      // Ensure the string is null-terminated
      model->comment[strlen(str) - i - 1] = '\0';
      return; // Exit after getting the comment
    }
  }
  // If there's no comment, set it to an empty string
  model->comment = strdup("");
}

void getTag(char* str, Lasm* model) {
  char** words = split(str);
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == ':') {
      // Allocate memory for model->tag
      model->tag = malloc(i + 1);
      strcpy(model->tag, words[0]);
      // Ensure the string is null-terminated
      model->tag[i+1] = '\0';
      return; // Exit after getting the tag
    }
  }
  // If there's no tag, set it to an empty string
  model->tag = strdup("");
}

void getOperator(char* str, Lasm* model) {
  char** words = split(str);
  if (strcmp(model->tag, words[0]) == 0) {
    model->operator = (char*)malloc((strlen(words[1]) + 1) * sizeof(char));
    strcpy(model->operator, words[1]);
    free(words);
    return;
  }
  else {
    model->operator = malloc((strlen(words[0]) + 1) * sizeof(char));
    strcpy(model->operator, words[0]);
    free(words);
    return;
  }
  model->operator = strdup("");
  free(words);
}

void getOperand(char* str, Lasm* model) {
    // Call the split function to tokenize the input string
    char** words = split(str);
    
    // Check if the second word in the string is not the same as the tag
    if (strcmp(model->tag, words[1]) != 0) {
        // If the second word is not the tag, it's considered as the operand
        model->operand = (char*)malloc((strlen(words[1]) + 1) * sizeof(char));
        strcpy(model->operand, words[1]);
    } else {
        // If the second word is the tag, then there is no operand
        model->operand = strdup("");
    }
    
    // Free the memory allocated for splitting the string into words
    free(words);
}

void print(Lasm model) {
  printf("Метка: %s, Оператор: %s, Операнд: %s, Комментарий: %s\n", model.tag, model.operator, model.operand, model.comment);
}

#endif
