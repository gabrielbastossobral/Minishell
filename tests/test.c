#include "minunit.h"
#include "../includes/minishell.h"

MU_TEST(check_quotes_basic_test)
{
	printf("------------------------------\n");
	printf("TESTE: Casos Básicos de Aspas\n");
	printf("------------------------------\n");

	mu_assert_int_eq(check_quotes('\"', 0), 1);  // Abre aspas duplas
	mu_assert_int_eq(check_quotes('\"', 1), 0);  // Fecha aspas duplas
	mu_assert_int_eq(check_quotes('\'', 0), 2);  // Abre aspas simples
	mu_assert_int_eq(check_quotes('\'', 2), 0);  // Fecha aspas simples
}

MU_TEST(check_quotes_edge_cases)
{
	printf("--------------------------------\n");
	printf("TESTE: Casos Limite e Erros\n");
	printf("--------------------------------\n");

	mu_assert_int_eq(check_quotes('\"', 2), 2);  // Aspas duplas abertas dentro de aspas simples
	mu_assert_int_eq(check_quotes('\'', 1), 1);  // Aspas simples abertas dentro de aspas duplas
	mu_assert_int_eq(check_quotes('a', 0), 0);   // Caractere comum sem aspas abertas
	mu_assert_int_eq(check_quotes('{', 1), 1);   // Caracteres aleatórios dentro de aspas duplas
	mu_assert_int_eq(check_quotes('}', 2), 2);   // Caracteres aleatórios dentro de aspas simples
}

MU_TEST(check_quotes_complex_cases)
{
	printf("---------------------------------\n");
	printf("TESTE: Casos Complexos e Aninhados\n");
	printf("---------------------------------\n");

	mu_assert_int_eq(check_quotes('\"', 0), 1);  // Abre aspas duplas
	mu_assert_int_eq(check_quotes('\'', 1), 1);  // Aspas simples dentro de aspas duplas (mantém aberto)
	mu_assert_int_eq(check_quotes('\"', 1), 0);  // Fecha aspas duplas

	mu_assert_int_eq(check_quotes('\'', 0), 2);  // Abre aspas simples
	mu_assert_int_eq(check_quotes('\"', 2), 2);  // Aspas duplas dentro de aspas simples (ignora)
	mu_assert_int_eq(check_quotes('\'', 2), 0);  // Fecha aspas simples
}

MU_TEST(insert_token_basic_test)
{
    t_token *head = NULL;

    insert_token(&head, "echo");
    mu_assert_string_eq(head->value, "echo");
    mu_assert_int_eq(head->type, 0);
    mu_assert(head->next == NULL, "Next should be NULL");
    mu_assert(head->prev == NULL, "Prev should be NULL");

    // Cleanup
    free(head);
}

MU_TEST(insert_token_multiple_test)
{
    t_token *head = NULL;

    insert_token(&head, "echo");
    insert_token(&head, "cd");
    insert_token(&head, "pwd");

    mu_assert_string_eq(head->value, "echo");
    mu_assert_string_eq(head->next->value, "cd");
    mu_assert_string_eq(head->next->next->value, "pwd");
    mu_assert(head->next->next->next == NULL, "Next of last token should be NULL");

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

MU_TEST(insert_token_existing_list_test)
{
    t_token *head = NULL;

    insert_token(&head, "echo");
    insert_token(&head, "cd");

    t_token *new_head = head;
    insert_token(&new_head, "pwd");

    mu_assert_string_eq(head->value, "echo");
    mu_assert_string_eq(head->next->value, "cd");
    mu_assert_string_eq(head->next->next->value, "pwd");
    mu_assert(head->next->next->next == NULL, "Next of last token should be NULL");

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}


MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(check_quotes_basic_test);
	MU_RUN_TEST(check_quotes_edge_cases);
	MU_RUN_TEST(check_quotes_complex_cases);
    MU_RUN_TEST(insert_token_basic_test);
    MU_RUN_TEST(insert_token_multiple_test);
    MU_RUN_TEST(insert_token_existing_list_test);
}

int main(void)
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}