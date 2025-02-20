#include "minunit.h"
#include "../includes/minishell.h"

int tests_run = 0;

MU_TEST(test_hello_world)
{
    const char *result = print_hello_world();
    mu_assert_string_eq(result, "Hello, World!\n");
    tests_run++;
}

MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(test_hello_world);
}

int main(int argc, char **argv)
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Total tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_run - minunit_fail);
    printf("Tests failed: %d\n", minunit_fail);
    return minunit_fail;
}