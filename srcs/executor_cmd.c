#include "../includes/minishell.h"

void free_pipe_fds(int **fds, int pipe_count)
{
    int i;

    if (!fds)
        return ;
    i = -1;
    while (++i < pipe_count)
        free(fds[i]);
    free(fds);
}

static void setup_redirections(t_data *data, int pipe_index)
{
    int pipe_count;

    pipe_count = data->exec.nbr_process - 1;    
    if (pipe_index > 0)
        dup2(data->exec.fds[pipe_index - 1][0], STDIN_FILENO);
    if (pipe_index < pipe_count)
        dup2(data->exec.fds[pipe_index][1], STDOUT_FILENO);
}

void child_process(t_data *data, int pipe_index)
{
    char **cmd;
    t_token *token_ptr;
    int i;

    i = -1;
    token_ptr = data->exec.tmp;
    while (++i < pipe_index && token_ptr) 
    {
        while (token_ptr && token_ptr->type != PIPE)
            token_ptr = token_ptr->next;
        if (token_ptr && token_ptr->type == PIPE)
            token_ptr = token_ptr->next;
    }
    data->exec.tmp = token_ptr;
    setup_redirections(data, pipe_index);
    close_all_fds(data->exec.fds, data->exec.nbr_process - 1);
    cmd = create_cmd_array(data->exec.tmp);
    if (data->exec.tmp->type == BUILDIN && execute_builtin(data, cmd))
    {
        free_matrix(cmd);
        exit(data->exit_error);
    }
    execute_external(data, cmd);
    free_matrix(cmd);
    exit(127);
}

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
