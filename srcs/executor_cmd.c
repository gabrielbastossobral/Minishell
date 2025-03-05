#include "../includes/minishell.h"

static int count_tokens_until_pipe(t_token *tokens)
{
    t_token *tmp;
    int     count;

    count = 0;
    tmp = tokens;
    while (tmp && tmp->type != PIPE)
    {
        count++;
        tmp = tmp->next;
    }
    return (count);
}

char **create_cmd_array(t_token *tokens)
{
    t_token *tmp;
    char    **cmd;
    int     count;
    int     i;

    count = count_tokens_until_pipe(tokens);
    cmd = malloc(sizeof(char *) * (count + 1));
    if (!cmd)
        handle_erros("Error: malloc failed", 0, NULL);
    i = 0;
    tmp = tokens;
    while (tmp && tmp->type != PIPE)
    {
        cmd[i] = ft_strdup(tmp->value);
        if (!cmd[i])
            handle_erros("Error: malloc failed", 0, NULL);
        i++;
        tmp = tmp->next;
    }
    cmd[i] = NULL;
    return (cmd);
}