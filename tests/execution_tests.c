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


MU_TEST(test_ls) 
{
    printf("--------------------------------------------------------------\n");
    printf("TESTE 1: Verificar se o comando ls é executado corretamente\n");
    printf("--------------------------------------------------------------\n");
    char *output = capture_output("ls");
    mu_check(output_contains(output, "minishell") || 
             output_contains(output, "Makefile"));
    free(output);
}

MU_TEST(test_ls_with_flags) {
    printf("--------------------------------------------------------------\n");
    printf("TESTE 2: Verificar se o comando ls -la é executado corretamente\n");
    printf("--------------------------------------------------------------\n");
    char *output = capture_output("ls -la");
    mu_check(output_contains(output, "total"));
    mu_check(output_contains(output, "."));
    free(output);
}

MU_TEST(test_ls_with_path) 
{
    printf("--------------------------------------------------------------\n");
    printf("TESTE 3: Verificar se o comando ls /tmp é executado corretamente\n");
    printf("--------------------------------------------------------------\n");
    char *output = capture_output("ls /tmp");
    mu_check(!output_contains(output, "No such file or directory"));
    free(output);
}

MU_TEST(test_quoted_command) {
    printf("--------------------------------------------------------------\n");
    printf("TESTE 4: Verificar erro quando comando está entre aspas\n");
    printf("--------------------------------------------------------------\n");
    char *output = capture_output("\"/bin/echo\" Hello World");
    
    mu_check(output_contains(output, "No such file or directory"));    
    mu_check(!output_contains(output, "Hello World"));
    free(output);
}

MU_TEST(test_quoted_command_with_quotes) {
    printf("--------------------------------------------------------------\n");
    printf("TESTE 5: Verificar erro quando comando está entre aspas (com argumentos entre aspas)\n");
    printf("--------------------------------------------------------------\n");
    char *output = capture_output("\"/bin/echo\" \"Hello World\"");
    
    mu_check(output_contains(output, "No such file or directory"));    
    mu_check(!output_contains(output, "Hello World"));
    free(output);
}


MU_TEST_SUITE(test_suite2)
{
    MU_RUN_TEST(test_ls);
    MU_RUN_TEST(test_ls_with_flags);
    MU_RUN_TEST(test_ls_with_path);
    MU_RUN_TEST(test_quoted_command);
    MU_RUN_TEST(test_quoted_command_with_quotes);
}

int main(void)
{
    MU_RUN_SUITE(test_suite2);
    MU_REPORT();
    return MU_EXIT_CODE;
}