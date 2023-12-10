#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *oldValue = S21_NULL;
  if (!str && !oldValue) {
    return S21_NULL;
  }
  if (str) oldValue = str;
  // Пропускаем начальные разделители
  oldValue += s21_strspn(oldValue, delim);
  if (*oldValue == '\0') {
    oldValue = S21_NULL;
    return S21_NULL;
  }
  char *token_start = oldValue;
  oldValue += s21_strcspn(oldValue, delim);
  if (*oldValue != '\0') {
    *oldValue = '\0';
    oldValue++;
  }
  return token_start;
}