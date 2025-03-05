#include "../includes/minishell.h"

static void create_pipes(t_exec *ex)
{
    int i;
    int pipe_count;

    pipe_count = ex->nbr_process - 1;
    if (pipe_count <= 0)
        return ;
    ex->fds = malloc(sizeof(int *) * pipe_count);
    if (!ex->fds)
        handle_erros("Error: malloc failed", 0, NULL);
    i = 0;
    while (i < pipe_count)
    {
        ex->fds[i] = malloc(sizeof(int) * 2);
        if (!ex->fds[i] || pipe(ex->fds[i] == -1))
            handle_erros("Error: pipe failed", 0, NULL);
        i++;
    }
}

static int count_process(t_token *tokens)
{
    t_token *tmp;
    int count;

    count = 1;
    tmp = tokens;
    while (tmp)
    {
        if (tmp->type == PIPE)
            count++;
        tmp = tmp->next;
    }
    return (count);
}

t_exec init_executor(t_data *data)
{
    t_exec ex;

    ex.nbr_process = count_process(data->tokens);
    ex.pid = NULL;
    ex.fds = NULL;
    ex.status = 0;
    ex.tmp = data->tokens;
    create_pipes(&ex);
    return (ex);
}
