#include <stdlib.h>

#include "s21_string.h"

char make_char_lower(char c) {
  char result = c;
  if (c >= 'A' && c <= 'Z') result = c - ('A' - 'a');

  return result;
}

void *s21_to_lower(const char *str) {
  if (str == S21_NULL) return S21_NULL;
  size_t len = s21_strlen(str);
  char *line = malloc(sizeof(char) * len + 1);
  char *linePtr = line;
  if (line == S21_NULL) {
    return S21_NULL;
  }
  size_t i = 0;
  for (; i < len; i++) {
    *(line + i) = make_char_lower(*(str + i));
  }
  *(line + i) = '\0';
  return linePtr;
}
