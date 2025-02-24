#include "../includes/minishell.h"

void handle_erros(char *msg, int stage, void *ptr)
{
    int i;

    if (ptr)
    {
        free(ptr);
        {
            if (stage == 1)
            {
                char **arr = (char **)ptr;
                i = -1;
                while (arr[++i])
                    free(arr[i]);
                free(arr);
            }
            else
                free(ptr);
        }
    }
    if (!stage)
        ft_printf("%s%s%s\n", RED, msg, RESET);
}
