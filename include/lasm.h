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
    // Вызываем функцию split для токенизации входной строки
    char** words = split(str);
    
    // Инициализируем операнд пустой строкой
    model->operand = strdup("");
    
    // Находим индекс точки с запятой, обозначающей начало комментария
    int comment_index = -1;
    for (int i = 0; words[i] != NULL; i++) {
        if (strcmp(words[i], ";") == 0) {
            comment_index = i;
            break;
        }
    }

    // Находим индекс оператора
    int operator_index = -1;
    for (int i = 0; words[i] != NULL; i++) {
        if (strcmp(words[i], model->operator) == 0) {
            operator_index = i;
            break;
        }
    }
    
    // Конкатенируем слова между оператором и комментарием (если присутствует)
    if (operator_index != -1) {
        // Определяем начальный и конечный индексы для операнда
        int operand_start_index = operator_index + 1;
        int operand_end_index = (comment_index != -1) ? comment_index : operator_index + 1;
        
        // Выделяем память для операнда и конкатенируем слова
        size_t operand_length = 0;
        for (int i = operand_start_index; i < operand_end_index; i++) {
            operand_length += strlen(words[i]) + 1; // +1 для пробела между словами
        }
        model->operand = (char*)malloc((operand_length + 1) * sizeof(char)); // +1 для нулевого символа
        strcpy(model->operand, "");
        for (int i = operand_start_index; i < operand_end_index; i++) {
            strcat(model->operand, words[i]);
            strcat(model->operand, " ");
        }
        model->operand[operand_length] = '\0'; // Убеждаемся, что строка операнда завершена нулевым символом
    }
    
    // Освобождаем память, выделенную для токенизации строки на слова
    free(words);
}

void print(Lasm model) {
  printf("Метка: %s, Оператор: %s, Операнд: %s, Комментарий: %s\n", model.tag, model.operator, model.operand, model.comment);
}

#endif
