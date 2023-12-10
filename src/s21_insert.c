#include <stdlib.h>

#include "s21_string.h"

char* s21_insert(const char* src, const char* str, s21_size_t start_index) {
  if (src == NULL || str == NULL || start_index > s21_strlen(src)) {
    return NULL;
  }

  int src_len = s21_strlen(src);
  int str_len = s21_strlen(str);
  int index = start_index;
  int end_src_len = s21_strlen(src) - index;
  char* result = (char*)malloc((src_len + str_len + 1) * sizeof(char));
  if (result == NULL) {
    return NULL;
  }
  int i;
  for (i = 0; i < index && src[i] != '\0'; i++) {
    result[i] = src[i];
  }
  result[i] = '\0';
  s21_strncat(result, str, str_len);
  s21_strncat(result, src + index, end_src_len);

  return result;
}