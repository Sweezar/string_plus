#ifndef _S21_SPRINTF_H_
#define _S21_SPRINTF_H_

#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char c;
  long long d;
  long double e;
  int E;
  long double f;
  long double g;
  int G;
  int *n;
  long long o;
  int *p;
  char *s;
  long long x;
  int X;
  int fisNull;
  int oisNull;
  int xisNull;
  int uisNull;
  unsigned long long u;
} Specifiers_s;

typedef struct {
  int minus;
  int null;
  int plus;
  int sharp;
  int space;
} Flags_s;

typedef struct {
  int number;
} Width_s;

typedef struct {
  int number;
  int isNull;
} Accuracy_s;

typedef struct {
  int h;
  int l;
  int L;
} Length_s;

typedef struct {
  Specifiers_s specifiers;
  Accuracy_s accuracy;
  Flags_s flags;
  Length_s length;
  Width_s width;
  int isNegative;
} Arguments_s;

typedef struct {
  int nonPrintCh;
  int nonPrintChPos;
} NonPrintCh_s;

int s21_sprintf(char *str, const char *format, ...);
char *formatStringHandler(char *str, const char *format, Arguments_s *arguments,
                          va_list factor, char *startStr, NonPrintCh_s *npc);

char *specifiersHandler(char *str, const char *ch, Arguments_s *arguments,
                        va_list factor, char *startStr, NonPrintCh_s *npc);

char *cSpecifierHandler(char *str, Arguments_s *arguments, va_list factor,
                        char *startStr, NonPrintCh_s *npc);
char *diSpecifierHandler(char *str, Arguments_s *arguments, va_list factor);
char *eSpecifierHandler(char *str, Arguments_s *arguments, va_list factor);
char *fSpecifierHandler(char *str, Arguments_s *arguments, va_list factor);
void nSpecifierHandler(char *str, Arguments_s *arguments, va_list factor,
                       char *startStr);
char *oSpecifierHandler(char *str, Arguments_s *arguments, va_list factor);
char *pSpecifierHandler(char *str, Arguments_s *arguments, va_list factor);
char *sSpecifierHandler(char *str, Arguments_s *arguments, va_list factor);
char *uSpecifierHandler(char *str, Arguments_s *arguments, va_list factor);
char *xSpecifierHandler(char *str, Arguments_s *arguments, va_list factor);

void resetArguments(Arguments_s *arguments);

char *printSpecificatorC(char *str, Arguments_s *arguments, const char *string);
char *printSpecificatorD(char *str, Arguments_s *arguments, const char *string);
char *printSpecificatorE(char *str, Arguments_s *arguments, const char *string);
char *printSpecificatorF(char *str, Arguments_s *arguments, const char *string);
char *printSpecificatorO(char *str, Arguments_s *arguments, const char *string);
char *printSpecificatorP(char *str, Arguments_s *arguments, const char *string);
char *printSpecificatorS(char *str, Arguments_s *arguments);
char *printSpecificatorU(char *str, Arguments_s *arguments, const char *string);
char *printSpecificatorX(char *str, Arguments_s *arguments, const char *string);

const char *accuracyHandle(const char *ch, Arguments_s *arguments,
                           va_list factor);
const char *flagsHandler(const char *ch, Arguments_s *arguments);
const char *lengthHandle(const char *ch, Arguments_s *arguments);
const char *widthHandle(const char *ch, Arguments_s *arguments, va_list factor);

int nullsCounter(Arguments_s *arguments, const char *string);
int spacesCounter(Arguments_s *arguments, const char *string);

char *printNulls(char *str, Arguments_s *arguments, int nulls);
char *printSpaces(char *str, int spaces);

#endif