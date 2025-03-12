#include "../libs/libft/includes/not_in_standard_includes/ft_gc.h"
#include "minunit.h"
#include <stdio.h>
#include <string.h>

// Cores para formatação do output
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"
#define BOLD          "\x1b[1m"

static int count_nodes(void)
{
    t_gc **gc = get_global_s_gc();
    t_gc *current = *gc;
    int count = 0;

    while (current)
    {
        count++;
        current = current->next;
    }
    return (count);
}

// Função para imprimir detalhes do garbage collector
static void print_gc_info(const char *title)
{
    int nodes = count_nodes();
    printf("\n%s=== %s ===%s\n", BOLD, title, COLOR_RESET);
    printf("• Nós no garbage collector: %s%d%s\n", 
           (nodes > 0) ? COLOR_GREEN : COLOR_RED, nodes, COLOR_RESET);
}

// Teste de alocação básica
MU_TEST(test_gc_malloc)
{
    printf("\n\n%s%s★★★ TESTE: ALOCAÇÃO BÁSICA ★★★%s\n", BOLD, COLOR_BLUE, COLOR_RESET);
    print_gc_info("Estado Inicial");
    
    printf("\n%s1. Alocando uma string de 10 bytes%s\n", BOLD, COLOR_RESET);
    char *str = gc_malloc(10);
    
    printf("• Ponteiro alocado: %p\n", (void*)str);
    mu_check(str != NULL);
    printf("%s✓ Memória alocada com sucesso%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s2. Escrevendo e lendo dados%s\n", BOLD, COLOR_RESET);
    strcpy(str, "test");
    printf("• Valor escrito: \"%s\"\n", str);
    printf("• Valor esperado: \"test\"\n");
    mu_check(strcmp(str, "test") == 0);
    printf("%s✓ Dados podem ser escritos e lidos corretamente%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s3. Verificando registro no garbage collector%s\n", BOLD, COLOR_RESET);
    int nodes = count_nodes();
    printf("• Nós esperados: 1\n");
    printf("• Nós encontrados: %d\n", nodes);
    mu_check(nodes == 1);
    printf("%s✓ O nó foi registrado corretamente no garbage collector%s\n", COLOR_GREEN, COLOR_RESET);
    
    print_gc_info("Antes de gc_exit()");
    printf("\n%s4. Liberando toda a memória com gc_exit()%s\n", BOLD, COLOR_RESET);
    gc_exit();
    
    print_gc_info("Depois de gc_exit()");
    nodes = count_nodes();
    printf("• Nós esperados após gc_exit: 0\n");
    printf("• Nós encontrados: %d\n", nodes);
    mu_check(nodes == 0);
    printf("%s✓ Memória liberada com sucesso%s\n", COLOR_GREEN, COLOR_RESET);
}

// Teste de múltiplas alocações
MU_TEST(test_multiple_mallocs)
{
    printf("\n\n%s%s★★★ TESTE: MÚLTIPLAS ALOCAÇÕES ★★★%s\n", BOLD, COLOR_BLUE, COLOR_RESET);
    print_gc_info("Estado Inicial");
    
    printf("\n%s1. Alocando três variáveis inteiras%s\n", BOLD, COLOR_RESET);
    int *a = gc_malloc(sizeof(int));
    int *b = gc_malloc(sizeof(int));
    int *c = gc_malloc(sizeof(int));
    
    printf("• Ponteiros alocados: a=%p, b=%p, c=%p\n", (void*)a, (void*)b, (void*)c);
    mu_check(a != NULL && b != NULL && c != NULL);
    printf("%s✓ Todas as alocações tiveram sucesso%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s2. Atribuindo valores%s\n", BOLD, COLOR_RESET);
    *a = 1;
    *b = 2;
    *c = 3;
    printf("• Valores atribuídos: a=%d, b=%d, c=%d\n", *a, *b, *c);
    
    mu_check(*a == 1 && *b == 2 && *c == 3);
    printf("%s✓ Todos os valores foram armazenados corretamente%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s3. Verificando registro no garbage collector%s\n", BOLD, COLOR_RESET);
    int nodes = count_nodes();
    printf("• Nós esperados: 3\n");
    printf("• Nós encontrados: %d\n", nodes);
    mu_check(nodes == 3);
    printf("%s✓ Todos os nós foram registrados corretamente%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s4. Liberando toda a memória%s\n", BOLD, COLOR_RESET);
    gc_exit();
    nodes = count_nodes();
    mu_check(nodes == 0);
    printf("%s✓ Memória liberada com sucesso (%d nós restantes)%s\n", 
           COLOR_GREEN, nodes, COLOR_RESET);
}

// Teste de liberação individual
MU_TEST(test_gc_free)
{
    printf("\n\n%s%s★★★ TESTE: LIBERAÇÃO INDIVIDUAL ★★★%s\n", BOLD, COLOR_BLUE, COLOR_RESET);
    print_gc_info("Estado Inicial");
    
    printf("\n%s1. Alocando duas variáveis inteiras%s\n", BOLD, COLOR_RESET);
    int *a = gc_malloc(sizeof(int));
    int *b = gc_malloc(sizeof(int));
    
    *a = 42;
    *b = 24;
    printf("• Valores atribuídos: a=%d, b=%d\n", *a, *b);
    
    int before_nodes = count_nodes();
    printf("• Nós registrados: %d (esperado: 2)\n", before_nodes);
    mu_check(before_nodes == 2);
    
    printf("\n%s2. Liberando o ponteiro 'a'%s\n", BOLD, COLOR_RESET);
    gc_free(a);
    
    int after_nodes = count_nodes();
    printf("• Nós restantes: %d (esperado: 1)\n", after_nodes);
    mu_check(after_nodes == 1);
    printf("%s✓ Ponteiro liberado com sucesso%s\n", COLOR_GREEN, COLOR_RESET);
    
    // Não podemos acessar *a aqui porque foi liberado
    printf("• Valor de b ainda acessível: %d\n", *b);
    
    printf("\n%s3. Limpando recursos restantes%s\n", BOLD, COLOR_RESET);
    gc_exit();
    after_nodes = count_nodes();
    printf("• Nós finais: %d (esperado: 0)\n", after_nodes);
    mu_check(after_nodes == 0);
}

// Teste de adicionar ponteiro externo
MU_TEST(test_gc_add)
{
    printf("\n\n%s%s★★★ TESTE: ADIÇÃO DE PONTEIRO EXTERNO ★★★%s\n", BOLD, COLOR_BLUE, COLOR_RESET);
    print_gc_info("Estado Inicial");
    
    printf("\n%s1. Alocando memória externamente (malloc normal)%s\n", BOLD, COLOR_RESET);
    int *external = malloc(sizeof(int));
    *external = 99;
    printf("• Ponteiro externo: %p, valor: %d\n", (void*)external, *external);
    
    printf("\n%s2. Adicionando o ponteiro externo ao garbage collector%s\n", BOLD, COLOR_RESET);
    short added = gc_add(external);
    mu_check(added == TRUE);
    printf("%s✓ Ponteiro adicionado com sucesso (retorno: %d)%s\n", 
           COLOR_GREEN, added, COLOR_RESET);
    
    int nodes = count_nodes();
    printf("• Nós no garbage collector: %d (esperado: 1)\n", nodes);
    mu_check(nodes == 1);
    
    printf("\n%s3. Tentando adicionar o mesmo ponteiro novamente%s\n", BOLD, COLOR_RESET);
    added = gc_add(external);
    printf("• Retorno da função gc_add: %d\n", added);
    
    nodes = count_nodes();
    printf("• Nós no garbage collector: %d (esperado ainda: 1)\n", nodes);
    mu_check(nodes == 1);
    printf("%s✓ Não foram criados nós duplicados%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s4. Liberando todos os recursos%s\n", BOLD, COLOR_RESET);
    gc_exit();
    nodes = count_nodes();
    printf("• Nós restantes: %d (esperado: 0)\n", nodes);
    mu_check(nodes == 0);
}

// Teste de tratamento de erros com NULL
MU_TEST(test_null_handling)
{
    printf("\n\n%s%s★★★ TESTE: TRATAMENTO DE NULL ★★★%s\n", BOLD, COLOR_BLUE, COLOR_RESET);
    print_gc_info("Estado Inicial");
    
    printf("\n%s1. Tentando liberar um ponteiro NULL%s\n", BOLD, COLOR_RESET);
    printf("• Chamando gc_free(NULL) - deve ser tratado graciosamente\n");
    gc_free(NULL);  // Não deve causar crash
    printf("%s✓ Chamada com NULL tratada sem erros%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s2. Tentando adicionar um ponteiro NULL%s\n", BOLD, COLOR_RESET);
    short result = gc_add(NULL);
    printf("• Resultado de gc_add(NULL): %d (esperado: FALSE)\n", result);
    mu_check(result == FALSE);
    printf("%s✓ Tentativa de adicionar NULL rejeitada corretamente%s\n", COLOR_GREEN, COLOR_RESET);
    
    int nodes = count_nodes();
    printf("• Nós no garbage collector: %d (deve ser 0)\n", nodes);
    mu_check(nodes == 0);
}

// Teste com diferentes tipos de dados
MU_TEST(test_different_data_types)
{
    printf("\n\n%s%s★★★ TESTE: DIFERENTES TIPOS DE DADOS ★★★%s\n", BOLD, COLOR_BLUE, COLOR_RESET);
    print_gc_info("Estado Inicial");
    
    printf("\n%s1. Alocando diferentes tipos de dados%s\n", BOLD, COLOR_RESET);
    
    // Char
    char *c = gc_malloc(sizeof(char));
    *c = 'A';
    
    // Int array
    int *arr = gc_malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++)
        arr[i] = i + 1;
    
    // Float
    float *f = gc_malloc(sizeof(float));
    *f = 3.14159f;
    
    // String
    char *str = gc_malloc(20);
    strcpy(str, "Hello World");
    
    printf("• Char: '%c'\n", *c);
    printf("• Array: [%d, %d, %d, %d, %d]\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
    printf("• Float: %f\n", *f);
    printf("• String: \"%s\"\n", str);
    
    int nodes = count_nodes();
    printf("\n• Total de nós no garbage collector: %d (esperado: 4)\n", nodes);
    mu_check(nodes == 4);
    printf("%s✓ Todos os tipos de dados foram alocados com sucesso%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s2. Liberando todos os recursos%s\n", BOLD, COLOR_RESET);
    gc_exit();
    nodes = count_nodes();
    printf("• Nós restantes: %d (esperado: 0)\n", nodes);
    mu_check(nodes == 0);
}

// Teste de estrutura personalizada
typedef struct s_person {
    char *name;
    int age;
    float height;
} t_person;

MU_TEST(test_custom_struct)
{
    printf("\n\n%s%s★★★ TESTE: ESTRUTURAS PERSONALIZADAS ★★★%s\n", BOLD, COLOR_BLUE, COLOR_RESET);
    print_gc_info("Estado Inicial");
    
    printf("\n%s1. Alocando uma estrutura personalizada%s\n", BOLD, COLOR_RESET);
    t_person *person = gc_malloc(sizeof(t_person));
    
    printf("• Pessoa alocada em: %p\n", (void*)person);
    mu_check(person != NULL);
    
    printf("\n%s2. Alocando dados internos da estrutura%s\n", BOLD, COLOR_RESET);
    person->name = gc_malloc(20);
    strcpy(person->name, "John Doe");
    person->age = 30;
    person->height = 1.75f;
    
    printf("• Nome: %s\n", person->name);
    printf("• Idade: %d\n", person->age);
    printf("• Altura: %.2f m\n", person->height);
    
    int nodes = count_nodes();
    printf("\n• Total de nós no garbage collector: %d (esperado: 2)\n", nodes);
    mu_check(nodes == 2);
    printf("%s✓ Estrutura e seus dados alocados com sucesso%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s3. Liberando recursos%s\n", BOLD, COLOR_RESET);
    gc_exit();
    nodes = count_nodes();
    printf("• Nós restantes: %d (esperado: 0)\n", nodes);
    mu_check(nodes == 0);
}

// Teste de stress com muitas alocações
MU_TEST(test_stress)
{
    printf("\n\n%s%s★★★ TESTE: STRESS COM MUITAS ALOCAÇÕES ★★★%s\n", BOLD, COLOR_BLUE, COLOR_RESET);
    print_gc_info("Estado Inicial");
    
    int allocations = 1000;
    printf("\n%s1. Alocando %d blocos de memória%s\n", BOLD, allocations, COLOR_RESET);
    
    for (int i = 0; i < allocations; i++) {
        int *num = gc_malloc(sizeof(int));
        *num = i;
        
        // Exibir progresso periodicamente
        if (i % 100 == 0 || i == allocations - 1) {
            printf("• Progresso: %d/%d alocações (%d%%)\n", 
                   i + 1, allocations, ((i + 1) * 100) / allocations);
        }
    }
    
    int nodes = count_nodes();
    printf("\n• Total de nós no garbage collector: %d (esperado: %d)\n", 
           nodes, allocations);
    mu_check(nodes == allocations);
    printf("%s✓ Todas as alocações foram registradas com sucesso%s\n", COLOR_GREEN, COLOR_RESET);
    
    printf("\n%s2. Liberando todos os recursos%s\n", BOLD, COLOR_RESET);
    gc_exit();
    nodes = count_nodes();
    printf("• Nós restantes: %d (esperado: 0)\n", nodes);
    mu_check(nodes == 0);
    printf("%s✓ Memória liberada com sucesso após teste de stress%s\n", COLOR_GREEN, COLOR_RESET);
}

// Teste de uso após liberar (comportamento indesejado, apenas para demonstração)
MU_TEST(test_use_after_free)
{
    printf("\n\n%s%s★★★ TESTE: USO APÓS LIBERAÇÃO (PERIGOSO) ★★★%s\n", BOLD, COLOR_RED, COLOR_RESET);
    printf("%sAVISO: Este teste demonstra um comportamento inseguro%s\n", COLOR_YELLOW, COLOR_RESET);
    
    printf("\n%s1. Alocando e liberando memória%s\n", BOLD, COLOR_RESET);
    int *value = gc_malloc(sizeof(int));
    *value = 42;
    printf("• Valor inicial: %d\n", *value);
    
    gc_free(value);
    printf("• Memória liberada\n");
    
    printf("\n%s2. Tentando acessar memória liberada (NÃO FAÇA ISSO)%s\n", BOLD, COLOR_RESET);
    printf("• Tentando acessar valor: ");
    
    // AVISO: Esta parte é apenas para demonstração e pode causar comportamento indefinido
    printf("%s(Este valor não é confiável: %d)%s\n", 
           COLOR_RED, *value, COLOR_RESET);
    
    printf("\n%s✓ Teste concluído - Lembre-se de nunca acessar memória após liberá-la%s\n", 
           COLOR_YELLOW, COLOR_RESET);
    
    // Garantimos que não há vazamentos
    gc_exit();
}

MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(test_gc_malloc);
    MU_RUN_TEST(test_multiple_mallocs);
    MU_RUN_TEST(test_gc_free);
    MU_RUN_TEST(test_gc_add);
    MU_RUN_TEST(test_null_handling);
    MU_RUN_TEST(test_different_data_types);
    MU_RUN_TEST(test_custom_struct);
    MU_RUN_TEST(test_stress);
    MU_RUN_TEST(test_use_after_free);
}

int main(void)
{
    printf("%s%s\n===================================%s\n", 
           BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s  TESTES DO GARBAGE COLLECTOR  %s\n", 
           BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s===================================%s\n\n", 
           BOLD, COLOR_YELLOW, COLOR_RESET);
           
    MU_RUN_SUITE(test_suite);
    
    printf("\n%s%s===================================%s\n", 
           BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s  RESULTADO FINAL DOS TESTES  %s\n", 
           BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s===================================%s\n", 
           BOLD, COLOR_YELLOW, COLOR_RESET);
    
    MU_REPORT();
    return MU_EXIT_CODE;
}