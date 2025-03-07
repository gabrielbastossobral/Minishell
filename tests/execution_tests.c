#include "minunit.h"
#include "../includes/minishell.h"

MU_TEST_SUITE(test_suite2)
{

}

int main(void)
{
    MU_RUN_SUITE(test_suite2);
    MU_REPORT();
    return MU_EXIT_CODE;
}