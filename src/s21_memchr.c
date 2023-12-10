#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  s21_size_t i = 0;
  while (*((char *)str + i) && *((char *)str + i) != c && i < n) {
    ++i;
  }
  return c == *((char *)str + i) && n != 0 ? (char *)str + i : S21_NULL;
}