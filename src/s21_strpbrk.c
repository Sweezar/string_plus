#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *result = S21_NULL;
  int i = 0, j = 0;
  char c = 0;
  while (str1[i] && result == S21_NULL) {
    c = str1[i];
    while (str2[j] && str2[j] != c) {
      ++j;
    }
    if (c == str2[j]) {
      result = (char *)str1 + i;
    }
    j = 0;
    ++i;
  }
  return result;
}