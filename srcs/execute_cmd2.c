#include "../includes/minishell.h"

static void try_exec_direct(char **cmd, char **envp)
{
    if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/'))
    {
        if (access(cmd[0], X_OK) == 0)
            execve(cmd[0], cmd, envp);
    }
}

static void try_exec_with_path(char *cmd_path, char **cmd, char **envp)
{
    if (access(cmd_path, F_OK) == -1)
        return;
    if (access(cmd_path, X_OK) == -1)
        return;
    execve(cmd_path, cmd, envp);
}

static void try_env_paths(t_data *data, char **cmd)
{
    char *path;
    char **paths;
    char *full_path;
    char *tmp;
    int i;

    path = get_env_value("PATH", data->envp);
    if (!path)
        return ;
    paths = ft_split(path, ':');
    if (!paths)
        handle_erros("Error: malloc failed", 1, NULL);
    i = -1;
    while (paths[++i])
    {
        tmp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(tmp, cmd[0]);
        free(tmp);
        try_exec_with_path(full_path, cmd, data->envp);
        free(full_path);
    }
    free_matrix(paths);
}

void execute_external(t_data *data, char **cmd)
{
    if (!cmd || !cmd[0])
    {
        handle_erros("Error: command not found", 0, NULL);
        exit(127);
    }
    try_exec_direct(cmd, data->envp);
    try_env_paths(data, cmd);
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd[0], STDERR_FILENO);
    ft_putstr_fd(": command not found\n", STDERR_FILENO);
    exit(127);
}