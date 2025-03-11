#include "minunit.h"
#include "../includes/minishell.h"
#include "../libs/libft/includes/not_in_standard_includes/get_next_line.h"

t_token *create_test_tokens(char *input) {
    t_token *head = NULL;
    parser(&head, input);
    return head;
}

// Função para verificar se o parser tokenizou corretamente
int token_count(t_token *head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

// Função para liberar os tokens após o teste
void free_test_tokens(t_token *head) {
    t_token *temp;
    
    while (head) {
        temp = head;
        head = head->next;
        free(temp->value);
        free(temp);
    }
}

// Função para verificar o tipo de um token específico
int get_token_type_at_index(t_token *head, int index) {
    int i = 0;
    while (head && i < index) {
        head = head->next;
        i++;
    }
    return head ? head->type : -1;
}

// Função para verificar o valor de um token específico
char *get_token_value_at_index(t_token *head, int index) {
    int i = 0;
    while (head && i < index) {
        head = head->next;
        i++;
    }
    return head ? head->value : NULL;
}

// Teste para examinar detalhadamente a estrutura de tokens
MU_TEST(test_parser_token_analysis) {
    // String de comando com vários elementos para testar
    char *test_cmd = "echo hello | grep h > output.txt | cat < input.txt";
    t_token *tokens = create_test_tokens(test_cmd);
    
    // Contagem total de tokens
    int total = token_count(tokens);
    printf("\n\n==== ANÁLISE DO PARSER ====\n");
    printf("Comando: '%s'\n", test_cmd);
    printf("Total de tokens: %d\n\n", total);
    
    // Análise de cada token
    t_token *current = tokens;
    int i = 0;
    
    while (current) {
        printf("Token #%d:\n", i);
        printf("  Valor: '%s'\n", current->value);
        printf("  Tipo: %d (", current->type);
        
        // Traduz o código do tipo para texto
        switch (current->type) {
            case EXECVE: printf("EXECVE"); break;
            case BUILDIN: printf("BUILDIN"); break;
            case ARG: printf("ARG"); break;
            case PIPE: printf("PIPE"); break;
            case REDIR_OUT: printf("REDIR_OUT"); break;
            case REDIR_IN: printf("REDIR_IN"); break;
            case APPEND: printf("APPEND"); break;
            case HEREDOC: printf("HEREDOC"); break;
            case ARG_FILE: printf("ARG_FILE"); break;
            default: printf("DESCONHECIDO"); break;
        }
        printf(")\n");
        
        // Informação de token anterior e próximo
        printf("  Token anterior: %s\n", current->prev ? current->prev->value : "NULL");
        printf("  Próximo token: %s\n", current->next ? current->next->value : "NULL");
        printf("\n");
        
        current = current->next;
        i++;
    }
    
    // Limpa os tokens
    free_test_tokens(tokens);
}

MU_TEST_SUITE(test_suite2) {
    MU_RUN_TEST(test_parser_token_analysis);
}

int main(void) {
    MU_RUN_SUITE(test_suite2);
    MU_REPORT();
    return MU_EXIT_CODE;
}