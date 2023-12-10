GCC=gcc
CFALGS=-Wall -Werror -Wextra -std=c11
SOURCES=*.c
TESTS_SOURCES=*.c
OBJECTS=$(SOURCES:.c=.o)

ifeq ($(shell uname), Linux)
	CHECKFLAGS=-lcheck -lm -lpthread -lrt 
#	-lsubunit
else
	CHECKFLAGS=-lcheck -lm -lpthread
endif

all: clean s21_string.a 

s21_string.a:
	$(GCC) $(CFALGS) $(SOURCES) -c 
	ar rcs s21_string.a $(OBJECTS)

test: clean s21_string.a
	$(GCC) $(CFALGS) tests/$(TESTS_SOURCES) s21_string.a $(CHECKFLAGS) -o s21_string
	./s21_string

gcov_report: clean s21_string.a
	$(GCC) $(CFALGS) tests/$(TESTS_SOURCES) s21_string.a $(CHECKFLAGS) -fprofile-arcs -ftest-coverage -lgcov -o s21_string
	./s21_string
	lcov -t "./s21_string" -o report.info --no-external -c -d .
	genhtml -o report report.info
	open ./report/index.html

clean:
	rm -rf s21_string *.a $(OBJECTS) a.out *.gcno *.gcda report report.info