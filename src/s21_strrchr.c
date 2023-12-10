#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  int i = 0;
  int j = 0;
  char *returnCode = "";

  while (str[i]) {
    if (str[i] == c) {
      j = i;
    }
    ++i;
  }
  if (c != '\0') {
    returnCode = c == str[j] ? (char *)str + j : S21_NULL;
  }

  return returnCode;
}
