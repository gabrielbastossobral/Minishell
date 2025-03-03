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

	printf("---------------------------------\n");
	printf("TESTE: TOKEN BASIC\n");
	printf("---------------------------------\n");

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

	printf("---------------------------------\n");
	printf("TESTE: TOKEN MULTIPLE TEST\n");
	printf("---------------------------------\n");

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

	printf("---------------------------------\n");
	printf("TESTE: TOKEN EXISTING_LIST TEST\n");
	printf("---------------------------------\n");

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

MU_TEST(test_realloc_basic)
{

	printf("---------------------------------\n");
	printf("TESTE: REALLOC BASIC\n");
	printf("---------------------------------\n");

    char **split = NULL;

    // Alocação inicial
    split = ft_realloc(split, 2 * sizeof(char *));
    mu_assert(split != NULL, "Split should not be NULL after initial realloc");

    split[0] = ft_strdup("echo");
    split[1] = ft_strdup("hello");

    // Expansão
    split = ft_realloc(split, 3 * sizeof(char *));
    mu_assert(split != NULL, "Split should not be NULL after expansion");

    split[2] = ft_strdup("world");

    // Verificações
    mu_assert_string_eq(split[0], "echo");
    mu_assert_string_eq(split[1], "hello");
    mu_assert_string_eq(split[2], "world");

    // Cleanup
    for (int i = 0; i < 3; i++)
        free(split[i]);
    free(split);
}

MU_TEST(test_realloc_shrink)
{

	printf("---------------------------------\n");
	printf("TESTE: REALLOC SHRINK\n");
	printf("---------------------------------\n");

    char **split = NULL;

    // Aloca 4 posições
    split = ft_realloc(split, 4 * sizeof(char *));
    split[0] = ft_strdup("one");
    split[1] = ft_strdup("two");
    split[2] = ft_strdup("three");
    split[3] = ft_strdup("four");

    // Reduz o tamanho para 2 posições
    split = ft_realloc(split, 2 * sizeof(char *));
    mu_assert(split != NULL, "Split should not be NULL after shrinking");

    mu_assert_string_eq(split[0], "one");
    mu_assert_string_eq(split[1], "two");

    // Cleanup
    for (int i = 0; i < 2; i++)
        free(split[i]);
    free(split);
}

MU_TEST(test_realloc_null_pointer)
{

	printf("---------------------------------\n");
	printf("TESTE: REALLOC NULL POINTER\n");
	printf("---------------------------------\n");
	
    char *ptr = NULL;

    // Realoca ponteiro nulo (deve funcionar como malloc)
    ptr = ft_realloc(ptr, 10 * sizeof(char));
    mu_assert(ptr != NULL, "Realloc with NULL should behave like malloc");

    strcpy(ptr, "test");
    mu_assert_string_eq(ptr, "test");

    free(ptr);
}

MU_TEST(test_realloc_to_zero)
{

	printf("---------------------------------\n");
	printf("TESTE: REALLOC TO ZERO\n");
	printf("---------------------------------\n");

    char *ptr = malloc(10 * sizeof(char));
    strcpy(ptr, "data");

    // Realloc com tamanho zero (deve liberar e retornar NULL)
    ptr = ft_realloc(ptr, 0);
    mu_assert(ptr == NULL, "Realloc to 0 should return NULL");
}

MU_TEST(test_realloc_large)
{

	printf("---------------------------------\n");
	printf("TESTE: REALLOC LARGE\n");
	printf("---------------------------------\n");

    int *arr = NULL;

    // Aloca grande quantidade de memória
    arr = ft_realloc(arr, 1000 * sizeof(int));
    mu_assert(arr != NULL, "Large realloc should not return NULL");

    for (int i = 0; i < 1000; i++)
        arr[i] = i;

    for (int i = 0; i < 1000; i++)
        mu_assert_int_eq(arr[i], i);

    free(arr);
}

MU_TEST(is_builtin_basic_test)
{
    printf("---------------------------------\n");
    printf("TESTE: IS BUILTIN BASIC\n");
    printf("---------------------------------\n");

    // Teste básico para comandos builtin
    mu_assert_int_eq(is_builtin("echo"), 1);   // Comando builtin "echo"
    mu_assert_int_eq(is_builtin("cd"), 1);     // Comando builtin "cd"
    mu_assert_int_eq(is_builtin("pwd"), 1);    // Comando builtin "pwd"
    mu_assert_int_eq(is_builtin("export"), 1); // Comando builtin "export"
    mu_assert_int_eq(is_builtin("unset"), 1);  // Comando builtin "unset"
    mu_assert_int_eq(is_builtin("env"), 1);    // Comando builtin "env"
    mu_assert_int_eq(is_builtin("exit"), 1);   // Comando builtin "exit"
}

MU_TEST(is_builtin_non_builtin_test)
{
    printf("---------------------------------\n");
    printf("TESTE: IS BUILTIN NON-BUILTIN\n");
    printf("---------------------------------\n");

    // Teste para comandos que não são builtin
    mu_assert_int_eq(is_builtin("ls"), 0);     // Comando não builtin "ls"
    mu_assert_int_eq(is_builtin("grep"), 0);   // Comando não builtin "grep"
    mu_assert_int_eq(is_builtin("cat"), 0);    // Comando não builtin "cat"
    mu_assert_int_eq(is_builtin("make"), 0);   // Comando não builtin "make"
    mu_assert_int_eq(is_builtin("gcc"), 0);    // Comando não builtin "gcc"
    mu_assert_int_eq(is_builtin("vim"), 0);    // Comando não builtin "vim"
    mu_assert_int_eq(is_builtin("nano"), 0);   // Comando não builtin "nano"
}

MU_TEST(is_builtin_edge_cases_test)
{
    printf("---------------------------------\n");
    printf("TESTE: IS BUILTIN EDGE CASES\n");
    printf("---------------------------------\n");

    // Teste para casos limite
    mu_assert_int_eq(is_builtin(""), 0);       // String vazia
    mu_assert_int_eq(is_builtin(" "), 0);      // String com espaço
    mu_assert_int_eq(is_builtin("echoo"), 0);  // Comando similar mas não builtin
    mu_assert_int_eq(is_builtin("cd "), 0);    // Comando com espaço no final
    mu_assert_int_eq(is_builtin(" pwd"), 0);   // Comando com espaço no início
    mu_assert_int_eq(is_builtin("eChO"), 0);   // Comando com letras maiúsculas e minúsculas
}

MU_TEST(type_token_basic_test)
{
    printf("---------------------------------\n");
    printf("TESTE: TYPE TOKEN BASIC\n");
    printf("---------------------------------\n");

    // Teste básico para verificar a atribuição de tipos de token
    t_token *head = NULL;

    insert_token(&head, "echo");
    insert_token(&head, "|");
    insert_token(&head, "cd");

    type_token(&head);

    mu_assert_int_eq(head->type, BUILDIN); // "echo" deve ser BUILTIN
    mu_assert_int_eq(head->next->type, PIPE); // "|" deve ser PIPE
    mu_assert_int_eq(head->next->next->type, BUILDIN); // "cd" deve ser BUILTIN

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

MU_TEST(type_token_redirect_test)
{
    printf("---------------------------------\n");
    printf("TESTE: TYPE TOKEN REDIRECT\n");
    printf("---------------------------------\n");

    // Teste para verificar a atribuição de tipos de token de redirecionamento
    t_token *head = NULL;

    insert_token(&head, "echo");
    insert_token(&head, ">");
    insert_token(&head, "file.txt");

    type_token(&head);

    mu_assert_int_eq(head->type, BUILDIN); // "echo" deve ser BUILTIN
    mu_assert_int_eq(head->next->type, REDIRECT); // ">" deve ser REDIRECT
    mu_assert_int_eq(head->next->next->type, ARG_FILE); // "file.txt" deve ser ARG_FILE

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

MU_TEST(type_token_heredoc_test)
{
    printf("---------------------------------\n");
    printf("TESTE: TYPE TOKEN HEREDOC\n");
    printf("---------------------------------\n");

    // Teste para verificar a atribuição de tipos de token de HEREDOC
    t_token *head = NULL;

    insert_token(&head, "<<");
    insert_token(&head, "EOF");

    type_token(&head);

    mu_assert_int_eq(head->type, HEREDOC); // "<<" deve ser HEREDOC
    mu_assert_int_eq(head->next->type, ARG); // "EOF" deve ser ARG

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

MU_TEST(type_token_execve_test)
{
    printf("---------------------------------\n");
    printf("TESTE: TYPE TOKEN EXECVE\n");
    printf("---------------------------------\n");

    // Teste para verificar a atribuição de tipos de token EXECVE
    t_token *head = NULL;

    insert_token(&head, "ls");
    insert_token(&head, "-la");

    type_token(&head);

    mu_assert_int_eq(head->type, EXECVE); // "ls" deve ser EXECVE
    mu_assert_int_eq(head->next->type, ARG); // "-la" deve ser ARG

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

MU_TEST(test_split_line_arg)
{
    printf("---------------------------------\n");
    printf("TESTE: SPLIT LINE ARG\n");
    printf("---------------------------------\n");

    // Teste básico
    char *line1 = "echo hello world";
    char **split1 = split_line_arg(line1);

    mu_assert_string_eq(split1[0], "echo");
    mu_assert_string_eq(split1[1], "hello");
    mu_assert_string_eq(split1[2], "world");

    // Cleanup
    for (int i = 0; i < 3; i++)
        free(split1[i]);
    free(split1);

    // Teste com aspas
    char *line2 = "echo \"hello world\"";
    char **split2 = split_line_arg(line2);

    mu_assert_string_eq(split2[0], "echo");
    mu_assert_string_eq(split2[1], "\"hello world\"");

    // Cleanup
    for (int i = 0; i < 2; i++)
        free(split2[i]);
    free(split2);

    // Teste com caracteres especiais
    char *line3 = "echo hello;world";
    char **split3 = split_line_arg(line3);

    mu_assert_string_eq(split3[0], "echo");
    mu_assert_string_eq(split3[1], "hello;world");

    // Cleanup
    for (int i = 0; i < 2; i++)
        free(split3[i]);
    free(split3);

    // Teste com espaços em branco
    char *line4 = "   echo    hello   world   ";
    char **split4 = split_line_arg(line4);

    mu_assert_string_eq(split4[0], "echo");
    mu_assert_string_eq(split4[1], "hello");
    mu_assert_string_eq(split4[2], "world");

    // Cleanup
    for (int i = 0; i < 3; i++)
        free(split4[i]);
    free(split4);

    // Teste com aspas simples
    char *line5 = "echo 'hello world'";
    char **split5 = split_line_arg(line5);

    mu_assert_string_eq(split5[0], "echo");
    mu_assert_string_eq(split5[1], "'hello world'");

    // Cleanup
    for (int i = 0; i < 2; i++)
        free(split5[i]);
    free(split5);

    // Teste com espaços em branco no início e no final
    char *line6 = "   echo   ";
    char **split6 = split_line_arg(line6);

    mu_assert_string_eq(split6[0], "echo");

    // Cleanup
    for (int i = 0; i < 1; i++)
        free(split6[i]);
    free(split6);
}

MU_TEST(test_parser_basic)
{
    t_token *head = NULL;
    char *line = "echo hello world";

    int result = parser(&head, line);

    mu_assert_int_eq(result, 0);
    mu_assert_string_eq(head->value, "echo");
    mu_assert_int_eq(head->type, BUILDIN);
    mu_assert_string_eq(head->next->value, "hello");
    mu_assert_int_eq(head->next->type, ARG);
    mu_assert_string_eq(head->next->next->value, "world");
    mu_assert_int_eq(head->next->next->type, ARG);
    mu_assert(head->next->next->next == NULL, "Expected NULL at the end of the token list");

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}

MU_TEST(test_parser_redirection)
{
    t_token *head = NULL;
    char *line = "echo hello > output.txt";

    int result = parser(&head, line);

    mu_assert_int_eq(result, 0);
    mu_assert_string_eq(head->value, "echo");
    mu_assert_int_eq(head->type, BUILDIN);
    mu_assert_string_eq(head->next->value, "hello");
    mu_assert_int_eq(head->next->type, ARG);
    mu_assert_string_eq(head->next->next->value, ">");
    mu_assert_int_eq(head->next->next->type, REDIRECT);
    mu_assert_string_eq(head->next->next->next->value, "output.txt");
    mu_assert_int_eq(head->next->next->next->type, ARG_FILE);
    mu_assert(head->next->next->next->next == NULL, "Expected NULL at the end of the token list");

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}

MU_TEST(test_parser_pipe)
{
    t_token *head = NULL;
    char *line = "echo hello | grep world";

    int result = parser(&head, line);

    mu_assert_int_eq(result, 0);
    mu_assert_string_eq(head->value, "echo");
    mu_assert_int_eq(head->type, BUILDIN);
    mu_assert_string_eq(head->next->value, "hello");
    mu_assert_int_eq(head->next->type, ARG);
    mu_assert_string_eq(head->next->next->value, "|");
    mu_assert_int_eq(head->next->next->type, PIPE);
    mu_assert_string_eq(head->next->next->next->value, "grep");
    mu_assert_int_eq(head->next->next->next->type, EXECVE);
    mu_assert_string_eq(head->next->next->next->next->value, "world");
    mu_assert_int_eq(head->next->next->next->next->type, ARG);
    mu_assert(head->next->next->next->next->next == NULL, "Expected NULL at the end of the token list");

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}

MU_TEST(test_parser_builtin)
{
    t_token *head = NULL;
    char *line = "cd /home/user";

    int result = parser(&head, line);

    mu_assert_int_eq(result, 0);
    mu_assert_string_eq(head->value, "cd");
    mu_assert_int_eq(head->type, BUILDIN);
    mu_assert_string_eq(head->next->value, "/home/user");
    mu_assert_int_eq(head->next->type, ARG);
    mu_assert(head->next->next == NULL, "Expected NULL at the end of the token list");

    // Cleanup
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}

MU_TEST(test_syntax_checker_basic)
{
    t_data data;
    t_token *head = NULL;
    char *line = "echo hello world";

    data.tokens = head;
    int result = parser(&data.tokens, line);
    mu_assert_int_eq(result, 0);

    result = syntax_checker(&data);
    mu_assert_int_eq(result, 0);

    // Cleanup
    t_token *tmp;
    while (data.tokens)
    {
        tmp = data.tokens;
        data.tokens = data.tokens->next;
        free(tmp->value);
        free(tmp);
    }
}

MU_TEST(test_syntax_checker_pipe_error)
{
    t_data data;
    t_token *head = NULL;
    char *line = "echo hello || world";

    data.tokens = head;
    int result = parser(&data.tokens, line);
    mu_assert_int_eq(result, 0);

    result = syntax_checker(&data);
    mu_assert_int_eq(result, 2);

    // Cleanup
    t_token *tmp;
    while (data.tokens)
    {
        tmp = data.tokens;
        data.tokens = data.tokens->next;
        free(tmp->value);
        free(tmp);
    }
}

MU_TEST(test_syntax_checker_redirect_error)
{
    t_data data;
    t_token *head = NULL;
    char *line = "echo hello > | world";

    data.tokens = head;
    int result = parser(&data.tokens, line);
    mu_assert_int_eq(result, 0);

    result = syntax_checker(&data);
    mu_assert_int_eq(result, 2);

    // Cleanup
    t_token *tmp;
    while (data.tokens)
    {
        tmp = data.tokens;
        data.tokens = data.tokens->next;
        free(tmp->value);
        free(tmp);
    }
}

MU_TEST(test_syntax_checker_backslash_error)
{
    t_data data;
    t_token *head = NULL;
    char *line = "echo hello \\ world";

    data.tokens = head;
    int result = parser(&data.tokens, line);
    mu_assert_int_eq(result, 0);

    result = syntax_checker(&data);
    mu_assert_int_eq(result, 2);

    // Cleanup
    t_token *tmp;
    while (data.tokens)
    {
        tmp = data.tokens;
        data.tokens = data.tokens->next;
        free(tmp->value);
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
	MU_RUN_TEST(test_realloc_basic);
	MU_RUN_TEST(test_realloc_shrink);
	MU_RUN_TEST(test_realloc_null_pointer);
	MU_RUN_TEST(test_realloc_to_zero);
	MU_RUN_TEST(test_realloc_large);
    MU_RUN_TEST(is_builtin_basic_test);
    MU_RUN_TEST(is_builtin_non_builtin_test);
    MU_RUN_TEST(is_builtin_edge_cases_test);
    MU_RUN_TEST(type_token_basic_test);
    MU_RUN_TEST(type_token_redirect_test);
    MU_RUN_TEST(type_token_heredoc_test);
    MU_RUN_TEST(type_token_execve_test);
    MU_RUN_TEST(test_split_line_arg);
    MU_RUN_TEST(test_parser_basic);
    MU_RUN_TEST(test_parser_redirection);
    MU_RUN_TEST(test_parser_pipe);
    MU_RUN_TEST(test_parser_builtin);
    MU_RUN_TEST(test_syntax_checker_basic);
    MU_RUN_TEST(test_syntax_checker_pipe_error);
    MU_RUN_TEST(test_syntax_checker_redirect_error);
    MU_RUN_TEST(test_syntax_checker_backslash_error);
}

int main(void)
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}