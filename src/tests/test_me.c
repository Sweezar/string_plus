#include "test_me.h"

int main(void) {
  int failed = 0;
  Suite *s21_string_test[] = {test_memchr(),
                              test_memcmp(),
                              test_memcpy(),
                              test_memset(),
                              test_strncat(),
                              test_strchr(),
                              test_strncmp(),
                              test_strncpy(),
                              test_strcspn(),
                              test_strerror(),
                              test_strlen(),
                              test_strpbrk(),
                              test_strrchr(),
                              test_strstr(),
                              test_strtok(),
                              test_to_lower(),
                              test_to_upper(),
                              test_insert(),
                              test_trim(),
                              test_sprintf_c(),
                              test_sprintf_d(),
                              test_sprintf_e(),
                              test_sprintf_f(),
                              test_sprintf_g(),
                              test_sprintf_hex1(),
                              test_sprintf_hex2(),
                              test_sprintf_i(),
                              test_sprintf_n(),
                              test_sprintf_octal(),
                              test_sprintf_percent(),
                              test_sprintf_pointer(),
                              test_sprintf_string(),
                              test_sprintf_unsigned(),
                              NULL};

  for (int i = 0; s21_string_test[i] != NULL; i++) {
    SRunner *sr = srunner_create(s21_string_test[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  printf("========= FAILED: %d =========\n", failed);

  return 0;
}