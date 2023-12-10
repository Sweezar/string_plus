#include <errno.h>
#include <stdlib.h>

#include "s21_string.h"
#include "s21_utils.h"

int flag_simbol(const char *trim_chars, char src) {
  for (s21_size_t i = 0; i < s21_strlen(trim_chars); i++) {
    if (trim_chars[i] == src) return 1;
  }
  return 0;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == S21_NULL) return S21_NULL;

  s21_size_t start_str = 0;
  s21_size_t end_str = s21_strlen(src) - 1;
  while (start_str < s21_strlen(src) &&
         flag_simbol(trim_chars, src[start_str])) {
    start_str++;
  }

  while (end_str > start_str && flag_simbol(trim_chars, src[end_str])) {
    end_str--;
  }
  char *result = (char *)malloc((end_str - start_str + 1) * sizeof(char) + 1);
  if (result == S21_NULL) {
    printError(errno);
  }
  s21_strncpy(result, src + start_str, end_str - start_str + 1);
  result[end_str - start_str + 1] = '\0';

  return (char *)result;
}
