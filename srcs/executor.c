#include "../includes/minishell.h"

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
    close_all_fds(&ex);
    wait_for_children(&ex, data);
    cleanup_executor(&ex); 
}