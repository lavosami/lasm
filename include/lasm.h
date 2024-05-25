#include "HashTable.h"
#include "configuration.h"

char* size;

int stringReverse(const char* s) {
  const char* p = s + strlen(s) - 1;
  while (s < p) {
    if (*p-- != *s++) {
      return 0;
    }
  }
  return 1;
}

char* decToHex(int decimal) {
  char* hexadecimal = malloc(sizeof(char) * 20);
  int index = 0;

  if (decimal == 0) {
    hexadecimal[index++] = '0';
  } else {
    while (decimal > 0) {
      int remainder = decimal % 16;
      if (remainder < 10) {
        hexadecimal[index++] = remainder + '0';
      } else {
        hexadecimal[index++] = remainder - 10 + 'A';
      }
      decimal /= 16;
    }
  }

  if (index == 0) {
    hexadecimal[index++] = '0';
  }
  hexadecimal[index] = '\0';
  stringReverse(hexadecimal);
  return hexadecimal;
}

int hexToDec(char* number) {
  int decNumber = 0;
  for (int i = 0; i < strlen(number); i++) {
    if (isdigit(number[i])) {
      decNumber += (int)(number[i] - '0') * pow(16, strlen(number) - i - 1);
    }
    if (!isdigit(number[i]) && number[i] != '-') {
      decNumber +=
          (int)(number[i] - 'A' + 10) * pow(16, strlen(number) - i - 1);
    }
  }
  return decNumber;
}

void initMnemonicsTable(HashTable* mnemonics) {
  add("BEGIN", "-1", 1, mnemonics);
  add("END", "-1", 1, mnemonics);
  add("DB", "-1", 1, mnemonics);
  add("DW", "-1", 1, mnemonics);
  add("RB", "-1", 1, mnemonics);
  add("RW", "-1", 1, mnemonics);
  add("MOV", "10", 0, mnemonics);
  add("ADD", "11", 0, mnemonics);
  add("SUB", "12", 0, mnemonics);
  add("CMP", "13", 0, mnemonics);
  add("MUL", "14", 0, mnemonics);
  add("DIV", "15", 0, mnemonics);
  add("JE", "16", 0, mnemonics);
  add("LOOP", "17", 0, mnemonics);
  add("CALL", "18", 0, mnemonics);
  add("RET", "19", 0, mnemonics);
  add("HLT", "1A", 0, mnemonics);
}

void firstParse(FILE* fp, HashTable* names) {
  int counter = 0;             // counter of solutions
  int begin = 0;               // address of program begin
  int commentStart = 32;       // begin os comment
  char* label;                 // label str
  char* operator;              // operator str
  char* operands;              // operands str
  char* comment;               // comment str
  char codeLine[256];          // line of asm program
  const char splits[] = " \t"; // split characters

  while (fgets(codeLine, 256, fp)) {
    bool isOperator = false;
    label = NULL;
    operator= NULL;
    operands = NULL;
    comment = NULL;

    if (codeLine[strlen(codeLine) - 1] == '\n') {
      codeLine[strlen(codeLine) - 1] = '\0';
    }

    if (strlen(codeLine) >= commentStart) {
      char* commentString = &codeLine[commentStart];
      comment = commentString + 1;
      *commentString = '\0';
    }

    char* code = strtok(codeLine, splits);
    if (code == NULL && comment == NULL) {
      continue;
    }

    while (code != NULL) {
      if (codeLine[strlen(code) - 1] == ':') {
        code[strlen(code) - 1] = '\0';
        label = code;
        code = strtok(NULL, splits);
      } else if (!isOperator) {
        operator= code;
        isOperator = true;
        code = strtok(NULL, splits);
      } else {
        operands = code;
        while (code[strlen(code) - 1] == ',') {
          code = strtok(NULL, splits);
          strcat(operands, code);
        }
        code = strtok(NULL, splits);
      }
    }

    counter += 3;
    if (operator!= NULL) {
      if (strcmp(operator, "BEGIN") == 0) {
        begin = hexToDec(operands);
        counter += begin - 6;
      }
      if (strcmp(operator, "END") == 0) {
        size = decToHex(counter - begin);
      }
    }
    if (operands != NULL) {
      const char splits2[] = ",";
      char* oper = malloc(sizeof(char) * strlen(operands));
      if (strchr(operands, ',') == NULL) {
        strcpy(oper, operands);
      } else {
        for (int i = 0; i < 2; i++) {
          if (i == 0) {
            oper = strtok(operands, splits2);
          } else {
            oper = strtok(NULL, splits2);
          }
          if (strcmp(oper, "ax") != 0 && strcmp(oper, "bx") != 0 &&
              strcmp(oper, "cx") != 0 && strcmp(oper, "dx") != 0) {
            if (oper != NULL || i == 0) {
              bool isDigit = true;
              for (int i = 0; i < strlen(oper); i++) {
                if (isdigit(oper[i]) == 0) {
                  isDigit = false;
                }
              }
              if (!isDigit) {
                DataItem* check = search(oper, names);
                if (check == NULL) {
                  char* temp = malloc(sizeof(char) * strlen(oper));
                  strcpy(temp, oper);
                  add(temp, decToHex(counter), 0, names);
                }
              }
            }
          }
        }
      }
    }
    if (label != NULL) {
      DataItem* check = search(label, names);
      if (check == NULL) {
        char* temp = malloc(sizeof(char) * strlen(label));
        strcpy(temp, label);
        add(temp, decToHex(counter), 1, names);
      } else if (strcmp(check->key, label) == 0 && check->mark == 0) {
        check->mark = 1;
        check->info = decToHex(counter);
      }
    }

    printf("Метка: %20s, Оператор: %20s, Операнд: %20s, Комментарий: %s\n",
           label, operator, operands, comment);
  }
}

void secondParse(FILE* fp, HashTable* names, HashTable* mnemonics) {
  FILE* outputFile = fopen("result.txt", "w"); // open file to write
  int begin;                                   // start position
  int counter = 0;                             // counter of solutions
  int objectByteCounter = 0;
  int commentStart = 32;
  char* label;
  char* operator;
  char* operands;
  char* comment;
  char codeLine[256];
  const char splits[] = " \t";
  char objectCodeString[256];
  char dump[256];

  objectCodeString[0] = '\0';
  dump[0] = '\0';

  while (fgets(codeLine, 256, fp)) {
    printf("\nCurrent line: %s   ", codeLine);
    bool isOperator = false;
    label = operator= operands = comment = NULL;

    if (codeLine[strlen(codeLine) - 1] == '\n') {
      codeLine[strlen(codeLine) - 1] = '\0';
    }

    if (strlen(codeLine) >= commentStart) {
      char* commentString = &codeLine[commentStart];
      comment = commentString + 1;
      *commentString = '\0';
    }

    char* code = strtok(codeLine, splits);
    if (code == NULL && comment == NULL) {
      continue;
    }

    while (code != NULL) {
      if (code[strlen(code) - 1] == ':') {
        code[strlen(code) - 1] = '\0';
        label = code;
        code = strtok(NULL, splits);
      } else if (!isOperator) {
        operator= code;
        isOperator = true;
        code = strtok(NULL, splits);
      } else {
        operands = code;
        while (code[strlen(code) - 1] == ',') {
          code = strtok(NULL, splits);
          strcat(operands, code);
        }
        code = strtok(NULL, splits);
      }
    }

    counter += 3;
    if (counter != 3) {
      printf("Counter of solutions: %s   ", decToHex(counter));
    }
    if (operator!= NULL) {
      if (strcmp(operator, "BEGIN") == 0) {
        begin = hexToDec(operands);
        counter += begin - 6;

        fprintf(outputFile, "Address of the begin: %sh\n", operands);
        fprintf(outputFile, "Entry point: %sh\n", operands);
        fprintf(outputFile, "Object code:\n:02%s02", operands);

        for (int i = 0; i < 4 - strlen(size); i++) {
          fprintf(outputFile, "0");
        }

        fprintf(outputFile, "%sXX\n", size);
        continue;
      } else if (strcmp(operator, "END") == 0) {
        strcat(objectCodeString, "XX\n");
        fputs(objectCodeString, outputFile);
        fprintf(outputFile, ":00%s01XX", decToHex(begin));
        fprintf(outputFile, "\nSize of the program: %sh\n", size);
        fprintf(outputFile, "Dump: %s", dump);
        return;
      } else {
        objectByteCounter++;

        switch (objectByteCounter) {
        case 1: {
          strcat(objectCodeString, ":03");
          strcat(objectCodeString, decToHex(counter));
          strcat(objectCodeString, "00");
          break;
        }

        case 2: {
          objectCodeString[2] = '6';
          break;
        }

        case 3: {
          objectByteCounter = 1;
          strcat(objectCodeString, "XX\n");
          fputs(objectCodeString, outputFile);
          objectCodeString[0] = '\0';
          break;
        }
        default:
          break;
        }

        DataItem* temp = search(operator, mnemonics);
        if (temp != NULL) {
          printf("Machine command: ");

          if (temp->mark != 1) {
            printf("%s", temp->info);
            strcat(dump, temp->info);
            strcat(objectCodeString, temp->info);
          } else {
            strcat(dump, "00");
            strcat(objectCodeString, "00");
            printf("00");
          }
        } else {
          printf("Error.");
          return;
        }
      }
    }
    if (operands != NULL) {
      bool print = false;
      const char splits2[] = ",";
      char* oper = malloc(sizeof(char) * strlen(operands));

      if (strchr(operands, ',') == NULL) {
        strcpy(oper, operands);
        bool isDigit = true;

        for (int i = 0; i < strlen(oper); i++) {
          if (isdigit(oper[i]) == 0) {
            isDigit = false;
          }
        }

        if (!isDigit) {
          if (search(oper, names) != NULL) {
            if (search(oper, names)->hashKey >= 0) {
              printf("%s", search(oper, names)->info);
              strcat(dump, search(oper, names)->info);
              strcat(objectCodeString, search(oper, names)->info);
              print = true;
            }
          }
        } else {
          for (int i = 0; i < 4 - strlen(oper); i++) {
            printf("0");
            strcat(dump, "0");
            strcat(objectCodeString, "0");
          }
          strcat(dump, oper);
          strcat(objectCodeString, oper);
          printf("%s", oper);
          print = true;
        }
      } else {
        oper = strtok(operands, splits2);
        bool isDigit = true;

        for (int i = 0; i < strlen(oper); i++) {
          if (isdigit(oper[i]) == 0) {
            isDigit = false;
          }
        }

        if (!isDigit) {
          if (search(oper, names) != NULL) {
            if (search(oper, names)->hashKey >= 0) {
              strcat(dump, search(oper, names)->info);
              strcat(objectCodeString, search(oper, names)->info);
              printf("%s", search(oper, names)->info);
              print = true;
            }
          }
        } else {
          for (int i = 0; i < 4 - strlen(oper); i++) {
            strcat(dump, "0");
            strcat(objectCodeString, "0");
            printf("0");
          }

          strcat(dump, oper);
          strcat(objectCodeString, oper);
          printf("%s", oper);
          print = true;
        }
        oper = strtok(NULL, splits2);
        if (oper != NULL) {
          bool isDigit2 = true;

          for (int i = 0; i < strlen(oper); i++) {
            if (isdigit(oper[i]) == 0) {
              isDigit2 = false;
            }
          }

          if (!isDigit2) {
            if (search(oper, names) != NULL) {
              if (search(oper, names)->hashKey >= 0) {
                strcat(dump, search(oper, names)->info);
                strcat(objectCodeString, search(oper, names)->info);
                printf("%s", search(oper, names)->info);
                print = true;
              }
            }
          } else {
            for (int i = 0; i < 4 - strlen(oper); i++) {
              strcat(dump, "0");
              strcat(objectCodeString, "0");
              printf("0");
            }

            strcat(dump, oper);
            strcat(objectCodeString, oper);
            printf("%s", oper);
            print = true;
          }
        }
      }
      if (!print) {
        strcat(dump, "0000");
        strcat(objectCodeString, "0000");
        printf("0000");
      }
    } else if (operator!= NULL) {
      strcat(dump, "0000");
      strcat(objectCodeString, "0000");
      printf("0000");
    }
    printf("\n");
  }
}