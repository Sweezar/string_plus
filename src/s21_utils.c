#include "s21_utils.h"

#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"
void printError(int error) {
  printf("%s\n", s21_strerror(error));
  exit(1);
}

// считает количество % в строке формата
int formatCounter(const char *string) {
  int result = 0;
  for (s21_size_t i = 0; i < s21_strlen(string); i++) {
    if (*(string + i) == '%') {
      result++;
    }
  }

  return result;
}

// Преобразует int в строку и возвращает указатель на нее
char *intInChar(long long number) {
  int isNegative = 0;
  if (number < 0) {
    isNegative = 1;
    number *= -1;
  }
  int i = digitsInIntCounter(number);
  int endStr = i;
  char *result = (char *)calloc(i + 1, sizeof(char));
  if (result == S21_NULL) {
    printError(errno);
    exit(1);
  }

  int n = 0;

  if (isNegative) {
    i++;
  }
  while (i > 0) {
    n = number % 10;
    result[--i] = n + 48;
    number = number / 10;
  }
  if (isNegative) {
    result[0] = '-';
  }
  result[endStr] = '\0';

  return result;
}

char *unsignedInChar(unsigned long long number) {
  int i = digitsInIntCounter(number);
  int endStr = i;
  char *result = (char *)calloc(i + 1, sizeof(char));
  if (result == S21_NULL) {
    printError(errno);
    exit(1);
  }

  int n = 0;

  while (i > 0) {
    n = number % 10;
    result[--i] = n + 48;
    number = number / 10;
  }

  result[endStr] = '\0';

  return result;
}

// Возвращает количество цифр в целом числе
int digitsInIntCounter(unsigned long long n) {
  int i = 0;
  if (n / 10 == 0) {
    return 1;
  } else {
    while (n > 0) {
      n = n / 10;
      i++;
    }
    return i;
  }
}

// Преобразует double в строку и возвращает указатель на нее
char *doubleInChar(long double number) {
  long double beforeComma, afterComma;
  afterComma = modfl(number, &beforeComma);
  if (afterComma < 0) {
    afterComma *= -1;
  }
  char *result = malloc(sizeof(char));

  char *beforeCommaString = intInChar((long)beforeComma);
  char *beforePtr = beforeCommaString;
  int i = 0;

  while (*beforeCommaString) {
    result[i++] = *beforeCommaString++;
    result = realloc(result, sizeof(char) + i);
  }
  result[i++] = '.';
  result = realloc(result, sizeof(char) + i);

  afterComma += 0.0000000005;
  afterComma *= 1000000000;

  while ((int)afterComma % 10 == 0 && afterComma != 0) {
    afterComma /= 10;
  }

  char *afterCommaString = intInChar((long long)afterComma);
  char *afterPtr = afterCommaString;

  while (*afterCommaString) {
    result[i++] = *afterCommaString++;
    result = realloc(result, sizeof(char) + i);
  }
  result[i] = '\0';

  free(afterPtr);
  free(beforePtr);

  return result;
}

char *doubleInCharN(long double number, int n) {
  if (n == 0) {
    return intInChar((long long)number);
  }
  long double beforeComma, afterComma;
  afterComma = modfl(number, &beforeComma);
  if (afterComma < 0) {
    afterComma *= -1;
  }
  char *result = malloc(sizeof(char));

  char *beforeCommaString = intInChar(beforeComma);
  char *beforePtr = beforeCommaString;
  int i = 0;

  while (*beforeCommaString) {
    result = realloc(result, sizeof(char) + sizeof(char) * i);
    result[i++] = *beforeCommaString++;
  }
  result = realloc(result, sizeof(char) + sizeof(char) * i);
  result[i++] = '.';

  afterComma += 0.5 / pow(10, n);
  afterComma *= pow(10, n);

  char *afterCommaString = intInChar((long long)afterComma);
  char *afterPtr = afterCommaString;

  while (*afterCommaString) {
    result = realloc(result, sizeof(char) + sizeof(char) * i);
    result[i++] = *afterCommaString++;
  }
  result = realloc(result, sizeof(char) + sizeof(char) * i);
  result[i] = '\0';

  free(afterPtr);
  free(beforePtr);

  return result;
}

// Преобразование строки в число
int strInInt(char ch) {
  int result = -1;

  if (ch >= '0' && ch <= '9') {
    result = ch - 48;
  }
  return result;
}

// Преобразование десятичного числа в восьмеричное или шестнадцатиричное
char *conversionDexInHexOrOcta(long long number, int numeralSystem) {
  int i = 0;
  int isNegative = 0;
  if (0 > number) {
    isNegative = 1;
  }
  long long prevNumber = number;
  long long maxIntDiv = prevNumber;
  long long tmpNumber = 0;
  int *tmpResult = malloc(sizeof(int) * 1000);
  if (tmpResult == S21_NULL) printError(errno);
  if (number > 0 || number < 0) {
    while (maxIntDiv != 0) {
      if (tmpResult == S21_NULL) printError(errno);
      tmpNumber = maxIntDiv % numeralSystem;
      maxIntDiv = maxIntDiv / numeralSystem;
      prevNumber = maxIntDiv;
      tmpResult[i] = tmpNumber;
      i++;
    }
    i--;
  }
  int j = 0;
  char *result = malloc(sizeof(char) * i + 2);
  if (result == S21_NULL) printError(errno);

  while (i >= 0) {
    int shiftNumber = 0;
    if (isNegative) {
      shiftNumber = numeralSystem - 1;
      if (i == 0) shiftNumber++;
    }

    result[j] = (tmpResult[i] + shiftNumber > 9)
                    ? (char)(tmpResult[i] + shiftNumber + 87)
                    : (char)(tmpResult[i] + shiftNumber + 48);
    j++;
    i--;
  }
  result[j] = '\0';
  free(tmpResult);
  return result;
}

// ПРЕОБРАЗОВАНИЕ DEC В OCTA
char *octaIntInChar(long long number) {
  if (number == 0) {
    return "0";
  }
  int numeralSystem = 8;
  return conversionDexInHexOrOcta(number, numeralSystem);
}

// ПРЕОБРАЗОВАНИЕ DEC В HEXA
char *hexaIntInChar(long long number) {
  if (number == 0) {
    return "0";
  }
  int numeralSystem = 16;
  return conversionDexInHexOrOcta(number, numeralSystem);
}

char *reverseStr(char *str) {
  if (str == S21_NULL || *str == '\0') return S21_NULL;
  int end_str = s21_strlen(str);
  s21_size_t i = 0;
  char *result = (char *)malloc(sizeof(char) * end_str + 10);
  char *resultPtr = result;
  while (i < s21_strlen(str)) {
    result[i] = str[end_str - 1];
    i++;
    end_str--;
  }
  result[s21_strlen(str)] = '\0';

  return resultPtr;
}

// Преобразование адреса в строку
char *ptrInChar(int *address) {
  char *str = calloc(100, sizeof(char));
  char *ptr = str;

  s21_size_t *addressPtr = (s21_size_t *)address;
  if (addressPtr == NULL) {
    *str++ = '0';
  } else {
    while (addressPtr != 0) {
      s21_size_t lastSymbol = ((s21_size_t)addressPtr) % 16;
      lastSymbol < 10 ? (*str++ = 48 + lastSymbol) : (*str++ = 87 + lastSymbol);
      addressPtr = ((s21_size_t *)(((s21_size_t)addressPtr) >> 4));
    }
  }
  *str++ = 'x';
  *str++ = '0';
  *str = '\0';

  char *result = reverseStr(ptr);
  free(ptr);

  return result;
}

double fractionOfE(double number) {
  if (number == 0) return 0;
  int minus = 0;
  if (number < 0) {
    number = -number;
    minus = 1;
  }
  double result = number;

  if ((int)result == 0) {
    while ((int)result < 1) {
      result *= 10;
    }
  } else {
    while ((int)result / 10 != 0) {
      result /= 10;
    }
  }

  return (minus) ? -result : result;
}

int exponent(double number) {
  if (number == 0) return 0;
  if (number < 0) number = -number;

  int result = 0;
  if ((int)number == 0) {
    while ((int)number < 1) {
      number *= 10;
      result++;
    }
  } else {
    while ((int)number / 10 != 0) {
      number /= 10;
      result++;
    }
  }
  return result;
}

char *exponentOfE(double number) {
  int exponentNum = exponent(number);
  char *result = malloc(sizeof(char) * 30);
  char *resultPtr = result;
  if (result == S21_NULL) {
    printError(errno);
    exit(1);
  }

  *result++ = 'e';
  if ((int)number == 0 && number != 0) {
    *result++ = '-';
  } else {
    *result++ = '+';
  }

  if (exponentNum < 10) {
    *result++ = '0';
    *result++ = (char)exponentNum + 48;
  } else {
    char *exponentStr = intInChar(exponentNum);
    char *exponentStrPtr = exponentStr;
    while (*exponentStr) {
      *result++ = *exponentStr++;
    }
    free(exponentStrPtr);
  }

  *result = '\0';

  return resultPtr;
}

long double roundTo(long double number, int precision) {
  double div = 1.0;
  if (precision < 0) {
    while (precision++) {
      div /= 10.0;
    }
  } else {
    while (precision--) {
      div *= 10.0;
    }
  }
  return floorl(number * div + 0.5) / div;
}