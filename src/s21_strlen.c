#include "s21_string.h"

s21_size_t s21_strlen(const char* str) {
  s21_size_t length = 0;
  const char* ptr = str;
  if (ptr != S21_NULL)
    while (*ptr++) {
      length++;
    }
  return length;
}