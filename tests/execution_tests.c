#include "minunit.h"
#include "../includes/minishell.h"
#include "../libs/libft/includes/not_in_standard_includes/get_next_line.h"

static char *capture_output(char *command) {
    char filepath[100];
    sprintf(filepath, "/tmp/minishell_test_%d", getpid());
    
    char cmdfile[100];
    sprintf(cmdfile, "/tmp/minishell_cmd_%d", getpid());
    FILE *cmd_fp = fopen(cmdfile, "w");
    fprintf(cmd_fp, "%s\nexit\n", command);
    fclose(cmd_fp);
    
    char exec_cmd[256];
    sprintf(exec_cmd, "cd .. && ./minishell < %s > %s 2>&1", cmdfile, filepath);
    system(exec_cmd);
    
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        remove(cmdfile);
        return NULL;
    }
    
    char *line;
    char *result = ft_strdup(""); // Buffer vazio inicial
    
    while ((line = get_next_line(fd)) != NULL) {
        char *temp = result;
        result = ft_strjoin(result, line);
        free(temp);
        free(line);
    }
    
    close(fd);
    remove(cmdfile);
    remove(filepath);
    
    return result;
}

static int output_contains(const char *output, const char *text) {
    // Cópia segura para usar strtok
    char *output_copy = ft_strdup(output);
    char *line = strtok(output_copy, "\n");
    int found = 0;
    
    while (line) {
        // Ignora linhas com o prompt e outras saídas não relevantes
        if (strstr(line, "$MINI$HELL_DE_VILÃO$") == NULL && 
            strstr(line, "exit") == NULL) {
            if (strstr(line, text)) {
                found = 1;
                break;
            }
        }
        line = strtok(NULL, "\n");
    }
    
    free(output_copy);
    return found;
}


// Testes para comandos simples
MU_TEST(test_echo) {
    char *output = capture_output("echo Hello World");
    mu_check(output_contains(output, "Hello World"));
    free(output);
}

MU_TEST(test_ls) {
    char *output = capture_output("ls");
    // Verifica se o output contém pelo menos um arquivo que deve existir no diretório
    mu_check(output_contains(output, "minishell") || 
             output_contains(output, "Makefile"));
    free(output);
}

MU_TEST(test_pwd) {
    char *output = capture_output("pwd");
    // Verifica se o output contém parte do caminho esperado
    mu_check(output_contains(output, "Minishell"));
    free(output);
}

// Teste para comando com pipe
MU_TEST(test_pipe_simple) {
    char *output = capture_output("ls | grep Make");
    mu_check(output_contains(output, "Make"));
    free(output);
}

// Teste para comando inexistente
MU_TEST(test_command_not_found) {
    char *output = capture_output("notarealcommand");
    mu_check(output_contains(output, "command not found"));
    free(output);
}

// Testes para comandos com aspas
MU_TEST(test_echo_with_quotes) {
    // Aspas duplas devem preservar espaços mas permitir expansão de variáveis
    char *output = capture_output("echo \"Hello     World\"");
    mu_check(output_contains(output, "Hello     World"));
    free(output);
}

MU_TEST(test_echo_with_single_quotes) {
    // Aspas simples devem preservar tudo literalmente
    char *output = capture_output("echo 'Hello     World'");
    mu_check(output_contains(output, "Hello     World"));
    free(output);
}

MU_TEST(test_echo_with_mixed_quotes) {
    // Mistura de aspas para verificar o comportamento correto
    char *output = capture_output("echo \"Hello 'quoted' World\"");
    mu_check(output_contains(output, "Hello 'quoted' World"));
    free(output);
}

// Testes para expansão de variáveis
MU_TEST(test_env_variable_expansion) {
    char *output = capture_output("export TEST_VAR=testvalue; echo $TEST_VAR");
    mu_check(output_contains(output, "testvalue"));
    free(output);
}

MU_TEST(test_env_variable_in_quotes) {
    // Teste a expansão em aspas duplas com formato alternativo
    char *output1 = capture_output("export QUOTE_TEST=expanded && echo \"$QUOTE_TEST value\"");
    mu_check(output_contains(output1, "expanded value"));
    free(output1);
    
    // Teste a não-expansão em aspas simples
    char *output2 = capture_output("export QUOTE_TEST=expanded && echo '$QUOTE_TEST value'");
    mu_check(output_contains(output2, "$QUOTE_TEST value"));
    free(output2);
}

// Testes para comandos mais complexos com pipe
MU_TEST(test_multiple_pipes) {
    // Testa múltiplos pipes encadeados
    char *output = capture_output("ls -la | grep Make | wc -l");
    mu_check(output_contains(output, "1") || output_contains(output, "2")); // Deve encontrar 1 ou 2 linhas
    free(output);
}

MU_TEST(test_pipe_with_quotes) {
    // Teste de pipe com argumentos entre aspas
    char *output = capture_output("echo \"Hello | World\" | grep Hello");
    mu_check(output_contains(output, "Hello | World"));
    free(output);
}

// Testes para comandos built-in
MU_TEST(test_cd_command) {
    // Mudar para um diretório e confirmar com pwd
    capture_output("cd ..");
    char *output = capture_output("pwd");
    mu_check(output_contains(output, "projeto_Minishell"));
    free(output);
}

MU_TEST(test_export_command) {
    // Verifica se o export funciona corretamente
    capture_output("export TEST_EXPORT=success");
    char *output = capture_output("echo $TEST_EXPORT");
    mu_check(output_contains(output, "success"));
    free(output);
}

MU_TEST(test_unset_command) {
    // Primeiro define uma variável
    capture_output("export TO_UNSET=value");
    // Depois remove a variável
    capture_output("unset TO_UNSET");
    // Verifica se a variável foi removida
    char *output = capture_output("echo $TO_UNSET");
    mu_check(!output_contains(output, "value"));
    free(output);
}

// Teste para comandos com caracteres especiais
MU_TEST(test_special_chars) {
    // Teste com caracteres especiais e espaços
    char *output = capture_output("echo Hello\\ World");
    mu_check(output_contains(output, "Hello World"));
    free(output);
}

MU_TEST_SUITE(test_suite2)
{
    MU_RUN_TEST(test_echo);
    MU_RUN_TEST(test_ls);
    MU_RUN_TEST(test_pwd);
    MU_RUN_TEST(test_pipe_simple);
    MU_RUN_TEST(test_command_not_found);
    MU_RUN_TEST(test_echo_with_quotes);
    MU_RUN_TEST(test_echo_with_single_quotes);
    MU_RUN_TEST(test_echo_with_mixed_quotes);
    MU_RUN_TEST(test_env_variable_expansion);
    MU_RUN_TEST(test_env_variable_in_quotes);
    MU_RUN_TEST(test_multiple_pipes);
    MU_RUN_TEST(test_pipe_with_quotes);
    MU_RUN_TEST(test_cd_command);
    MU_RUN_TEST(test_export_command);
    MU_RUN_TEST(test_unset_command);
    MU_RUN_TEST(test_special_chars);
}

int main(void)
{
    MU_RUN_SUITE(test_suite2);
    MU_REPORT();
    return MU_EXIT_CODE;
}