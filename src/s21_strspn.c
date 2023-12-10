#include "s21_string.h"

s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t i = 0;
  int returnCode = -1;
  int x = 0, y = 0;

  while (*(str1 + x) && returnCode == -1) {
    while (*(str2 + y) && *(str1 + x) != *(str2 + y)) {
      y++;
    }
    if (*(str1 + x) == *(str2 + y)) {
      i++;
      x++;
      y = 0;
    } else {
      returnCode = i;
    }
  }

  if (*(str1 + x) == '\0') {
    returnCode = s21_strlen(str1);
  }

  return returnCode == -1 ? (s21_size_t)S21_NULL : (s21_size_t)returnCode;
}