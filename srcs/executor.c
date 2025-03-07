#include "../includes/minishell.h"

static void cleanup_executor(t_exec *ex)
{
    if (ex->pid)
        free(ex->pid);
    if (ex->fds)
        free_pipe_fds(ex->fds, ex->nbr_process - 1);
}

static void wait_for_children(t_exec *ex, t_data *data)
{
    int i;
    int status;

    i = -1;
    while (++i < ex->nbr_process)
    {
        waitpid(ex->pid[i], &status, 0);
        if (WIFEXITED(status))
            data->exit_error = WEXITSTATUS(status);
    }
}

static void create_child_process(t_data *data, t_exec *ex)
{
    int i;

    ex->tmp = data->tokens;
    i = -1;
    ex->pid = malloc(sizeof(pid_t) * ex->nbr_process);
    if (!ex->pid)
        handle_erros("Error: malloc failed", 0, NULL);
    while (++i < ex->nbr_process)
    {
        ex->pid[i] = fork();
        if (ex->pid[i] == 0)
            child_process(data, i);
        advance_to_next_cmd(ex);
    }
}

void executor(t_data *data)
{
    t_exec ex;
    char **cmd;
    int is_builtin;

    ex = init_executor(data);
    data->exec = ex;
    if (ex.nbr_process == 1)
    {
        cmd = create_cmd_array(data->tokens);
        is_builtin = execute_builtin(data, cmd);
        free_matrix(cmd);
        if (is_builtin)
            return ;
    }
    create_child_process(data, &ex);
    close_all_fds(ex.fds, ex.nbr_process -1);
    wait_for_children(&ex, data);
    cleanup_executor(&ex); 
}