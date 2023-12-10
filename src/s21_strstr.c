#include <stdio.h>

#include "s21_string.h"

int compare(const char *x, const char *y) {
  int result = 1;
  while (*x && *y && result) {
    if (*x != *y) {
      result = 0;
    }
    if (result) {
      x++;
      y++;
    }
  }
  result = (*y == '\0');
  return result;
}

const char *s21_strstr(const char *haystack, const char *needle) {
  const char *result = S21_NULL;
  if (*needle == '\0') {
    result = haystack;
  }
  while (*haystack != '\0' && result == S21_NULL) {
    if (*haystack == *needle && compare(haystack, needle)) {
      result = haystack;
    }
    haystack++;
  }

  return result;
}