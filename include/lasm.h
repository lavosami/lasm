#ifndef HEADER_FILE
#define HEADER_FILE

#define comSymb ';'

#include <string.h>
#include <stdlib.h>

typedef struct Lasm {
   char* lable;
   char* oper;
   char* operand;
   char* comment;
} Lasm;

char* substring(char* s, int p, int l) {
   int c = 0;
   char* sub = (char*)malloc(sizeof(char) * l);
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
   return sub;
}

char* findComment(char* str) {
   for (int i = 0; i < strlen(str); i++) {
      if (str[i] == comSymb) {
         return substring(str, i + 2, strlen(str)-i + 1);
      }
   }

   return "";
}

#endif