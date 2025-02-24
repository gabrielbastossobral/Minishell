#include "minunit.h"
#include "../includes/minishell.h"

MU_TEST(check_quotes_test)
{
	printf("--------------------\n");
	printf("TEST: check_quotes_test\n");
	printf("--------------------\n");

    mu_assert_int_eq(check_quotes('\"', 0), 1);
    mu_assert_int_eq(check_quotes('\"', 1), 0);
    mu_assert_int_eq(check_quotes('\'', 0), 2);
    mu_assert_int_eq(check_quotes('\'', 2), 0);
}

MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(check_quotes_test);
}

int main(void)
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}