#include "../includes/minishell.h"

void handle_erros(char *msg, int stage)
{
    if (!stage)
        ft_printf("%s%s%s\n", RED, msg, RESET);
}