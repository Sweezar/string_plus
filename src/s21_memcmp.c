#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;

  while ((*((char *)str1) != '\0' || *((char *)str2) != '\0') && n != 0) {
    if (*((char *)str1) != *((char *)str2)) {
      result = *((char *)str1) - *((char *)str2);
      n = 0;
    } else {
      n--;
      str1++;
      str2++;
    }
  }
  return result;
}
