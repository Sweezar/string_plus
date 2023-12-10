#include "s21_sprintf.h"

#include "s21_string.h"
#include "s21_utils.h"

int s21_sprintf(char *str, const char *format, ...) {
  Arguments_s arguments = {0};
  NonPrintCh_s npc;
  npc.nonPrintCh = 0;
  npc.nonPrintChPos = -1;

  va_list factor;
  va_start(factor, format);

  char *startStr = str;

  str = formatStringHandler(str, format, &arguments, factor, startStr, &npc);
  int strLen = s21_strlen(startStr) + npc.nonPrintCh;
  if (npc.nonPrintCh) {
    *(startStr + npc.nonPrintChPos) = '\0';
  }
  va_end(factor);

  return strLen;
}

char *formatStringHandler(char *str, const char *format, Arguments_s *arguments,
                          va_list factor, char *startStr, NonPrintCh_s *npc) {
  for (const char *ch = format; *ch; ch++) {
    if (*ch != '%') {
      *str++ = *ch;
    } else {
      ch = flagsHandler(ch, arguments);
      ch = widthHandle(ch, arguments, factor);
      ch = accuracyHandle(ch, arguments, factor);
      ch = lengthHandle(ch, arguments);

      str = specifiersHandler(str, ch, arguments, factor, startStr, npc);
    }
    resetArguments(arguments);
  }
  *str = '\0';

  return str;
}

char *specifiersHandler(char *str, const char *ch, Arguments_s *arguments,
                        va_list factor, char *startStr, NonPrintCh_s *npc) {
  switch (*ch) {
    case 'i':
    case 'd':
      str = diSpecifierHandler(str, arguments, factor);
      break;
    case 'f':
      str = fSpecifierHandler(str, arguments, factor);
      break;
    case 'c':
      str = cSpecifierHandler(str, arguments, factor, startStr, npc);
      break;
    case 's':
      str = sSpecifierHandler(str, arguments, factor);
      break;
    case 'u':
      str = uSpecifierHandler(str, arguments, factor);
      break;
    case 'G':
    case 'g':
      if (*ch == 'G') arguments->specifiers.G = 1;
      break;
    case 'o':
      str = oSpecifierHandler(str, arguments, factor);
      break;
    case 'X':
    case 'x':
      if (*ch == 'X') arguments->specifiers.X = 1;
      str = xSpecifierHandler(str, arguments, factor);
      break;
    case 'n':
      nSpecifierHandler(str, arguments, factor, startStr);
      break;
    case 'p':
      str = pSpecifierHandler(str, arguments, factor);
      break;
    case 'E':
    case 'e':
      if (*ch == 'E') arguments->specifiers.E = 1;
      str = eSpecifierHandler(str, arguments, factor);
      break;
    case '%':
      *str++ = *ch;
      break;
    default:
      break;
  }

  return str;
}

/*str - Форматная строка
arguments - структура различных аргументов аргументов
factor - подаваемое число */

// Обработка спецификатора %d
char *diSpecifierHandler(char *str, Arguments_s *arguments, va_list factor) {
  if (arguments->length.h) {
    arguments->specifiers.d = (short)va_arg(factor, int);
  } else if (arguments->length.l) {
    arguments->specifiers.d = (long)va_arg(factor, long int);
  } else {
    arguments->specifiers.d = va_arg(factor, int);
  }

  if (arguments->specifiers.d < 0) {
    arguments->isNegative = 1;
    arguments->specifiers.d *= -1;
  }
  char *dString = intInChar(arguments->specifiers.d);

  str = printSpecificatorD(str, arguments, dString);
  free(dString);

  return str;
}

// Обработка спецификатора %f
char *fSpecifierHandler(char *str, Arguments_s *arguments, va_list factor) {
  if (!arguments->accuracy.isNull && arguments->accuracy.number == 0) {
    arguments->accuracy.number = 6;
  }
  if (arguments->length.L) {
    arguments->specifiers.f = (long double)va_arg(factor, long double);
  } else {
    arguments->specifiers.f = va_arg(factor, double);
  }
  if (arguments->specifiers.f < 0) {
    arguments->isNegative = 1;
    arguments->specifiers.f *= -1;
  }
  if (arguments->specifiers.f == 0) {
    arguments->specifiers.fisNull = 1;
  }

  arguments->specifiers.f =
      roundTo(arguments->specifiers.f, arguments->accuracy.number);
  char *fString =
      doubleInCharN(arguments->specifiers.f, arguments->accuracy.number);

  str = printSpecificatorF(str, arguments, fString);
  free(fString);

  return str;
}

// Обработка спецификатора %c
char *cSpecifierHandler(char *str, Arguments_s *arguments, va_list factor,
                        char *startStr, NonPrintCh_s *npc) {
  arguments->specifiers.c = va_arg(factor, int);
  if (arguments->specifiers.c == 0) {
    npc->nonPrintCh++;
    if (npc->nonPrintChPos == -1) {
      npc->nonPrintChPos = str - startStr;
    }
  }

  str = printSpecificatorC(str, arguments, &(arguments->specifiers.c));

  return str;
}

// Обработка спецификатора %s
char *sSpecifierHandler(char *str, Arguments_s *arguments, va_list factor) {
  arguments->specifiers.s = (char *)va_arg(factor, char *);

  if (arguments->specifiers.s == S21_NULL) {
    arguments->flags.space = 0;
    arguments->flags.null = 0;
    arguments->specifiers.s = "(null)";
  }

  if (!arguments->accuracy.number && !arguments->accuracy.isNull) {
    arguments->accuracy.number = s21_strlen(arguments->specifiers.s);
  }
  str = printSpecificatorS(str, arguments);

  return str;
}

// Обработка спецификатора %u
char *uSpecifierHandler(char *str, Arguments_s *arguments, va_list factor) {
  if (arguments->length.h) {
    arguments->specifiers.u = (unsigned short)va_arg(factor, unsigned int);
  } else if (arguments->length.l) {
    arguments->specifiers.u =
        (unsigned long long)va_arg(factor, unsigned long long);
  } else {
    arguments->specifiers.u = va_arg(factor, unsigned int);
  }
  if (arguments->specifiers.u == 0) {
    arguments->specifiers.uisNull = 1;
  }
  char *uString = unsignedInChar(arguments->specifiers.u);
  str = printSpecificatorU(str, arguments, uString);

  free(uString);

  return str;
}

// Обработка спецификатора %o
char *oSpecifierHandler(char *str, Arguments_s *arguments, va_list factor) {
  if (arguments->length.h) {
    arguments->specifiers.o = (short)va_arg(factor, int);
  } else if (arguments->length.l) {
    arguments->specifiers.o = (long long)va_arg(factor, long long);
  } else {
    arguments->specifiers.o = va_arg(factor, int);
  }

  char *oString = octaIntInChar(arguments->specifiers.o);
  if (arguments->specifiers.o == 0) {
    arguments->specifiers.oisNull = 1;
  }
  if (0 > arguments->specifiers.o) {
    arguments->isNegative = 1;
  }

  str = printSpecificatorO(str, arguments, oString);

  if (arguments->specifiers.o != 0) {
    free(oString);
  }
  return str;
}

// Обработка спецификатора %x
char *xSpecifierHandler(char *str, Arguments_s *arguments, va_list factor) {
  if (arguments->length.h) {
    arguments->specifiers.x = (short)va_arg(factor, int);
  } else if (arguments->length.l) {
    arguments->specifiers.x = (long)va_arg(factor, long);
  } else {
    arguments->specifiers.x = va_arg(factor, int);
  }
  char *xString = hexaIntInChar(arguments->specifiers.x);
  if (arguments->specifiers.x == 0) {
    arguments->specifiers.xisNull = 1;
  }
  if (0 > arguments->specifiers.x) {
    arguments->isNegative = 1;
  }

  if (arguments->specifiers.X) {
    char *XString = s21_to_upper(xString);
    str = printSpecificatorX(str, arguments, XString);
    free(XString);
  } else {
    str = printSpecificatorX(str, arguments, xString);
  }
  if (arguments->specifiers.x != 0) {
    free(xString);
  }
  return str;
}

void nSpecifierHandler(char *str, Arguments_s *arguments, va_list factor,
                       char *startStr) {
  arguments->specifiers.n = va_arg(factor, int *);
  int value = str - startStr;

  *arguments->specifiers.n = value;
}

// // Обработка спецификатора %p
char *pSpecifierHandler(char *str, Arguments_s *arguments, va_list factor) {
  arguments->specifiers.p = va_arg(factor, int *);

  char *pString = ptrInChar(arguments->specifiers.p);

  if (arguments->specifiers.p == S21_NULL) {
    if (arguments->accuracy.number || arguments->accuracy.isNull) {
      *(pString + 2 + arguments->accuracy.number) = '\0';
    }
  }

  str = printSpecificatorP(str, arguments, pString);

  free(pString);

  return str;
}

// Обработка спецификатора %e
char *eSpecifierHandler(char *str, Arguments_s *arguments, va_list factor) {
  if (!arguments->accuracy.isNull && arguments->accuracy.number == 0)
    arguments->accuracy.number = 6;
  if (arguments->length.L)
    arguments->specifiers.e = (long double)va_arg(factor, long double);
  else
    arguments->specifiers.e = va_arg(factor, double);
  if (arguments->specifiers.e < 0) {
    arguments->isNegative = 1;
    arguments->specifiers.e *= -1;
  }
  double fraction;
  fraction =
      roundTo(fractionOfE(arguments->specifiers.e), arguments->accuracy.number);
  char *fractionStr = doubleInChar(fraction);
  char *fractionStrPtr = fractionStr;
  char *eString = calloc(1000, sizeof(char));
  if (eString == S21_NULL) {
    printError(errno);
    exit(1);
  }
  int i = 0;
  while (*fractionStr) {
    *(eString + i) = *fractionStr++;
    i++;
  }
  free(fractionStrPtr);
  while ((int)s21_strlen(s21_strchr(eString, '.')) <=
         arguments->accuracy.number) {
    *(eString + i) = '0';
    i++;
  }
  char *exponent = exponentOfE(arguments->specifiers.e);
  char *exponentPtr = exponent;
  while (*exponent) {
    *(eString + i) = *exponent++;
    i++;
  }
  *(eString + i) = '\0';
  free(exponentPtr);
  if (arguments->specifiers.E) {
    char *EString = s21_to_upper(eString);
    str = printSpecificatorE(str, arguments, EString);
    free(EString);
  } else
    str = printSpecificatorE(str, arguments, eString);
  free(eString);
  return str;
}

const char *flagsHandler(const char *ch, Arguments_s *arguments) {
  ch = ch + 1;
  while (*ch == '-' || *ch == '+' || *ch == ' ' || *ch == '0' || *ch == '#') {
    switch (*ch) {
      case '-':
        arguments->flags.minus = 1;
        ch = ch + 1;
        break;
      case '+':
        arguments->flags.plus = 1;
        arguments->flags.space = 0;
        ch = ch + 1;
        break;
      case ' ':
        if (!(arguments->flags.plus)) {
          arguments->flags.space = 1;
        }
        ch = ch + 1;
        break;
      case '0':
        arguments->flags.null = 1;
        ch = ch + 1;
        break;
      case '#':
        arguments->flags.sharp = 1;
        ch = ch + 1;
        break;
      default:
        break;
    }
  }

  return ch;
}

void resetArguments(Arguments_s *arguments) {
  s21_memset(arguments, 0, sizeof(Arguments_s));
}

const char *widthHandle(const char *ch, Arguments_s *arguments,
                        va_list factor) {
  while (*ch >= '0' && *ch <= '9') {
    if (arguments->width.number == 0) {
      arguments->width.number = strInInt(*ch);
    } else {
      arguments->width.number = arguments->width.number * 10 + strInInt(*ch);
    }
    ch = ch + 1;
  }

  if (*ch == '*') {
    arguments->width.number = va_arg(factor, int);
    ch = ch + 1;
  }

  return ch;
}

char *printSpecificatorD(char *str, Arguments_s *arguments,
                         const char *string) {
  int spaces = spacesCounter(arguments, string);
  if (arguments->accuracy.isNull && arguments->specifiers.d == 0) {
    spaces++;
  }
  int nulls = nullsCounter(arguments, string);
  spaces = spaces - nulls;
  if (!arguments->flags.minus) {
    if (arguments->width.number || arguments->accuracy.number) {
      str = printSpaces(str, spaces);
    }
  }

  if (arguments->flags.plus && !arguments->isNegative) {
    *str++ = '+';
  }
  if (arguments->flags.space && !arguments->isNegative) {
    *str++ = ' ';
  }

  if (arguments->isNegative) {
    *str++ = '-';
  }

  if ((arguments->width.number && !arguments->flags.minus) ||
      arguments->accuracy.number) {
    str = printNulls(str, arguments, nulls);
  }

  // Проверяем если точность != 0 и значение != 0
  if (!(arguments->accuracy.isNull && arguments->specifiers.d == 0)) {
    // то добавляем в строку
    while (*string) {
      *str++ = *string++;
    }
  }

  if (arguments->width.number && arguments->flags.minus) {
    str = printSpaces(str, spaces);
  }

  return str;
}

char *printSpecificatorS(char *str, Arguments_s *arguments) {
  char *string = arguments->specifiers.s;
  int spaces = spacesCounter(arguments, string);
  int nulls = nullsCounter(arguments, string);
  if (arguments->flags.null) {
    spaces = spaces - nulls;
  }
  if (arguments->flags.space) {
    if (arguments->accuracy.isNull ||
        arguments->accuracy.number != (int)s21_strlen(string)) {
      *str++ = ' ';
    }
  }
  if (arguments->flags.null) {
    if (arguments->accuracy.number > (int)s21_strlen(string)) {
      *str++ = '0';
    }
  }

  if (arguments->width.number && !arguments->flags.minus) {
    if (!arguments->flags.null) {
      str = printSpaces(str, spaces);
    } else {
      str = printNulls(str, arguments, spaces);
    }
  }

  /* Строку печатаем полностью или если задана точность, то определенное
   количество символов */
  if (string != S21_NULL) {
    while (*string && arguments->accuracy.number &&
           !arguments->accuracy.isNull) {
      *str++ = *string++;
      arguments->accuracy.number--;
    }
  }
  if (arguments->width.number && arguments->flags.minus) {
    str = printSpaces(str, spaces);
  }

  return str;
}

char *printSpecificatorF(char *str, Arguments_s *arguments,
                         const char *string) {
  int spaces = spacesCounter(arguments, string);
  int nulls = nullsCounter(arguments, string);
  spaces = spaces - nulls;

  if (!arguments->flags.minus && !arguments->flags.null) {
    str = printSpaces(str, spaces);
  }

  if (!arguments->isNegative) {
    if (arguments->flags.plus) {
      *str++ = '+';
    }
    if (arguments->flags.space) {
      *str++ = ' ';
    }
  } else {
    *str++ = '-';
  }

  if (arguments->width.number && arguments->flags.null &&
      !arguments->flags.minus) {
    str = printNulls(str, arguments, spaces);
  }

  while (*string &&
         (arguments->accuracy.number > 0 || arguments->accuracy.isNull)) {
    *str++ = *string++;
  }
  if (arguments->flags.sharp && arguments->accuracy.isNull) {
    *str++ = '.';
  }

  if (arguments->width.number && arguments->flags.minus) {
    str = printSpaces(str, spaces);
  }
  str = printNulls(str, arguments, nulls);
  return str;
}

char *printSpecificatorC(char *str, Arguments_s *arguments,
                         const char *string) {
  int spaces = spacesCounter(arguments, string);

  if (!arguments->flags.minus) {
    if (arguments->flags.null) {
      str = printNulls(str, arguments, spaces);
    } else {
      str = printSpaces(str, spaces);
    }
  }

  while (*string) {
    *str++ = *string++;
  }

  if (arguments->width.number && arguments->flags.minus) {
    str = printSpaces(str, spaces);
  }

  return str;
}

char *printSpecificatorU(char *str, Arguments_s *arguments,
                         const char *string) {
  int spaces = spacesCounter(arguments, string);
  if (arguments->accuracy.isNull && arguments->specifiers.u == 0) {
    spaces++;
  }
  int nulls = nullsCounter(arguments, string);
  spaces = spaces - nulls;

  if (!arguments->flags.minus) {
    if (arguments->width.number || arguments->accuracy.number) {
      str = printSpaces(str, spaces);
    }
  }

  if ((arguments->width.number && !arguments->flags.minus) ||
      arguments->accuracy.number) {
    str = printNulls(str, arguments, nulls);
  }

  // Проверяем если точность != 0 и значение != 0
  if (!(arguments->accuracy.isNull && arguments->specifiers.u == 0)) {
    // то добавляем в строку
    while (*string) {
      *str++ = *string++;
    }
  }

  if (arguments->width.number && arguments->flags.minus) {
    str = printSpaces(str, spaces);
  }

  return str;
}

char *printSpecificatorE(char *str, Arguments_s *arguments,
                         const char *string) {
  int isAfterComma = 0;
  int spaces = spacesCounter(arguments, string);
  int nulls = nullsCounter(arguments, string);
  spaces = spaces - nulls;
  if (!arguments->flags.minus && !arguments->flags.null)
    str = printSpaces(str, spaces);
  if (!arguments->isNegative) {
    if (arguments->flags.plus) *str++ = '+';
    if (arguments->flags.space) *str++ = ' ';
  } else
    *str++ = '-';
  if (arguments->flags.null) {
    if ((arguments->width.number && !arguments->flags.minus) ||
        arguments->accuracy.number)
      str = printNulls(str, arguments, nulls);
  }
  while (*string &&
         (arguments->accuracy.number > 0 || arguments->accuracy.isNull) &&
         !(*string == 'e' || *string == 'E')) {
    if (isAfterComma == 1) arguments->accuracy.number--;

    if (*string == '.') {
      isAfterComma = 1;
      if (arguments->flags.sharp && arguments->accuracy.isNull) {
        arguments->accuracy.number = -1;
        arguments->accuracy.isNull = 0;
      }
    }
    if (!arguments->flags.sharp && arguments->accuracy.isNull &&
        *string == '.') {
      string = string + 1;
      arguments->accuracy.number = -1;
      arguments->accuracy.isNull = 0;
    } else
      *str++ = *string++;
  }
  str = printNulls(str, arguments, arguments->accuracy.number);
  while (*string && !(*string == 'e' || *string == 'E')) string = string + 1;
  while (*string) *str++ = *string++;
  if (arguments->width.number && arguments->flags.minus)
    str = printSpaces(str, spaces);
  return str;
}

char *printSpecificatorP(char *str, Arguments_s *arguments,
                         const char *string) {
  int spaces = spacesCounter(arguments, string);
  int nulls = nullsCounter(arguments, string);
  if (arguments->specifiers.p == S21_NULL) {
    spaces += 2;
    nulls += 2;
  }
  spaces = spaces - nulls;

  if (!arguments->flags.minus && !arguments->flags.null) {
    str = printSpaces(str, spaces);
  }
  *str++ = *string++;

  while (*string) {
    if (*(string - 1) == 'x') {
      str = printNulls(str, arguments, nulls);
    }
    *str++ = *string++;
  }

  if (arguments->width.number && arguments->flags.minus) {
    str = printSpaces(str, spaces);
  }
  return str;
}

char *printSpecificatorX(char *str, Arguments_s *arguments,
                         const char *string) {
  int spaces = spacesCounter(arguments, string);
  int nulls = nullsCounter(arguments, string);
  spaces = spaces - nulls;

  if (arguments->width.number && !arguments->flags.minus)
    str = printSpaces(str, spaces);
  if (arguments->flags.sharp && !arguments->specifiers.xisNull) {
    *str++ = '0';
    if (arguments->specifiers.X) {
      *str++ = 'X';
    } else if (arguments->specifiers.x)
      *str++ = 'x';
  }
  if (arguments->accuracy.number || arguments->flags.null)
    str = printNulls(str, arguments, nulls);
  if (arguments->flags.minus && 0 > arguments->specifiers.x) {
    int y = 8;
    while (y > (int)s21_strlen(string)) {
      if (arguments->specifiers.X) {
        *str++ = 'F';
      } else {
        *str++ = 'f';
      }
      y--;
      spaces--;
    }
  }
  if (arguments->specifiers.x < 0 && !arguments->isNegative) {  // обработка ul
    int y = 16;
    while (y > (int)s21_strlen(string)) {
      *str++ = 'f';
      y--;
      spaces--;
    }
  }
  // Проверяем если точность != 0 и значение != 0
  if (!(arguments->accuracy.isNull && arguments->specifiers.x == 0)) {
    // то добавляем в строку
    while (*string) *str++ = *string++;
  }
  if (arguments->width.number && arguments->flags.minus)
    str = printSpaces(str, spaces);
  return str;
}

char *printSpecificatorO(char *str, Arguments_s *arguments,
                         const char *string) {
  int spaces = spacesCounter(arguments, string);
  int nulls = nullsCounter(arguments, string);
  spaces = spaces - nulls;
  if (arguments->width.number && !arguments->flags.minus)
    str = printSpaces(str, spaces);

  if (arguments->flags.sharp && !arguments->specifiers.oisNull &&
      !arguments->accuracy.number)
    *str++ = '0';

  if (arguments->accuracy.number || arguments->flags.null)
    str = printNulls(str, arguments, nulls);

  if (arguments->flags.minus && 0 > arguments->specifiers.o) {
    int y = 11;
    *str++ = '3';
    y--;
    spaces--;
    while (y > (int)s21_strlen(string)) {
      *str++ = '7';
      y--;
      spaces--;
    }
  }

  // Проверяем если точность != 0 и значение != 0
  if (!(arguments->accuracy.isNull && arguments->specifiers.o == 0)) {
    // то добавляем в строку
    while (*string) *str++ = *string++;
  }

  if (arguments->width.number && arguments->flags.minus)
    str = printSpaces(str, spaces);

  return str;
}

const char *accuracyHandle(const char *ch, Arguments_s *arguments,
                           va_list factor) {
  if (*ch == '.') {
    ch = ch + 1;
    // Если после точки идет 0 или сразу спецификатор
    if ((*ch == '0' || *ch > '9') && arguments->accuracy.number == 0) {
      arguments->accuracy.isNull = 1;
      return *ch == '0' ? ++ch : ch;
    }
    // Смотрим значение точности
    while (*ch >= '0' && *ch <= '9') {
      if (arguments->accuracy.number == 0) {
        arguments->accuracy.number = strInInt(*ch);
      } else {
        arguments->accuracy.number =
            arguments->accuracy.number * 10 + strInInt(*ch);
      }
      ch = ch + 1;
    }

    if (*ch == '*') {
      arguments->accuracy.number = va_arg(factor, int);
      ch = ch + 1;
    }
  }

  return ch;
}

const char *lengthHandle(const char *ch, Arguments_s *arguments) {
  switch (*ch) {
    case 'h':
      arguments->length.h = 1;
      ch = ch + 1;
      break;
    case 'l':
      arguments->length.l = 1;
      ch = ch + 1;
      break;
    case 'L':
      arguments->length.L = 1;
      ch = ch + 1;
      break;
    default:
      break;
  }

  return ch;
}

int spacesCounter(Arguments_s *arguments, const char *string) {
  int spaces = 0;
  if (arguments->width.number && !arguments->flags.null &&
      !arguments->specifiers.f && !arguments->specifiers.s) {
    spaces = arguments->width.number - s21_strlen(string);
  }
  if (arguments->specifiers.s) {
    spaces = (arguments->accuracy.number < (int)s21_strlen(string))
                 ? arguments->width.number - arguments->accuracy.number
                 : arguments->width.number - (int)s21_strlen(string);
    if (arguments->flags.plus) spaces++;
  }
  if (arguments->flags.sharp &&
      (arguments->specifiers.o || arguments->specifiers.x)) {
    spaces--;
    if (arguments->specifiers.x) spaces--;
  }
  if (arguments->specifiers.f) {
    spaces = arguments->width.number -
             (s21_strcspn(string, ".") + arguments->accuracy.number + 1);
    if (arguments->accuracy.isNull && !arguments->flags.sharp) spaces++;
  }
  if (arguments->isNegative && !arguments->specifiers.x &&
      !arguments->specifiers.o) {
    spaces--;
  } else if ((arguments->flags.space || arguments->flags.plus) &&
             !arguments->specifiers.u && !arguments->specifiers.x &&
             !arguments->specifiers.o)
    spaces--;

  if (arguments->specifiers.c) {
    spaces = arguments->width.number - 1;
  }
  if (arguments->specifiers.e && arguments->accuracy.isNull &&
      arguments->width.number) {
    spaces++;  // +2 из-за .0
    if (!arguments->flags.sharp) spaces++;
  }
  if (arguments->specifiers.xisNull || arguments->specifiers.oisNull) {
    if (arguments->accuracy.isNull) spaces = arguments->width.number;
  }
  return spaces;
}

int nullsCounter(Arguments_s *arguments, const char *string) {
  int nulls = 0;
  if (arguments->width.number && !arguments->flags.minus &&
      arguments->flags.null && !arguments->accuracy.number) {
    nulls = arguments->width.number - s21_strlen(string);
  } else if (arguments->accuracy.number) {
    nulls = arguments->accuracy.number - s21_strlen(string);
  }

  if (arguments->specifiers.e) {
    if (arguments->flags.null) {
      if (!arguments->accuracy.isNull) {
        nulls = arguments->width.number - s21_strlen(string);
      } else {
        nulls =
            arguments->width.number - (s21_strlen(string) - 2);  // -2 из-за .0
      }
    }
    if ((arguments->flags.space || arguments->flags.plus) &&
        !arguments->isNegative) {
      nulls--;
    }
  }

  if (!arguments->accuracy.isNull && !arguments->flags.sharp) {
    if (arguments->specifiers.f ||
        arguments->specifiers
            .fisNull) {  // test 38!!!!!!!!!! без этого ломается
      nulls = arguments->accuracy.number -
              (s21_strlen(s21_strchr(string, '.')) - 1);
    }
  }

  if (arguments->isNegative &&
      (!arguments->accuracy.number || arguments->specifiers.e)) {
    nulls--;
  }

  if (arguments->specifiers.p) {
    nulls =
        arguments->accuracy.number - (s21_strlen(s21_strchr(string, 'x')) - 1);
  }

  return nulls > 0 ? nulls : 0;
}

char *printSpaces(char *str, int spaces) {
  while (spaces > 0) {
    *str++ = ' ';
    spaces--;
  }

  return str;
}

char *printNulls(char *str, Arguments_s *arguments, int nulls) {
  while (nulls > 0 && (arguments->flags.null || arguments->accuracy.number)) {
    *str++ = '0';
    nulls--;
  }

  return str;
}
