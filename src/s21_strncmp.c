#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  while (n != 0) {
    if (*((char *)str1) != *((char *)str2)) {
      return *((char *)str1) - *((char *)str2);
    }
    n--;
    str1++;
    str2++;
  }

  return 0;
}