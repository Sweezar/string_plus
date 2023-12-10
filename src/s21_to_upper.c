#include <stdlib.h>

#include "s21_string.h"

char myToUpperChar(char c) {
  char result = c;
  if (c >= 'a' && c <= 'z') result = c - ('a' - 'A');
  return result;
}

void* s21_to_upper(const char* str) {
  if (str == S21_NULL) return S21_NULL;
  size_t len = s21_strlen(str);

  char* line = (char*)malloc(len + 1);
  if (line == S21_NULL) {
    return S21_NULL;
  }

  for (size_t i = 0; i < len; i++) {
    line[i] = myToUpperChar(str[i]);
  }
  line[len] = '\0';

  return (char*)line;
}
