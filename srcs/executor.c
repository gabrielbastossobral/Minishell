/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:36:31 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 09:40:13 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	cleanup_executor(t_exec *ex)
{
	if (ex->pid)
		free(ex->pid);
	if (ex->fds)
		free_pipe_fds(ex->fds, ex->nbr_process - 1);
}

static void	wait_for_children(t_exec *ex, t_data *data)
{
	int	i;
	int	status;

	i = -1;
	while (++i < ex->nbr_process)
	{
		waitpid(ex->pid[i], &status, 0);
		if (WIFEXITED(status))
			data->exit_error = WEXITSTATUS(status);
	}
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}

static void	create_child_process(t_data *data, t_exec *ex)
{
	int	i;

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

int	execute_builtin(t_data *data, char **cmd)
{
	if (!ft_strncmp(cmd[0], "echo", 5))
		ft_echo(data, cmd);
	else if (!ft_strncmp(cmd[0], "cd", 3))
		ft_cd(data, cmd, &data->envp);
	else if (!ft_strncmp(cmd[0], "pwd", 4))
		ft_pwd(data, cmd, data->envp);
	else if (!ft_strncmp(cmd[0], "export", 7))
		ft_export(data, cmd, &data->envp);
	else if (!ft_strncmp(cmd[0], "unset", 6))
		ft_unset(data, cmd, &data->envp);
	else if (!ft_strncmp(cmd[0], "env", 4))
		ft_env(data, cmd, &data->envp);
	else if (!ft_strncmp(cmd[0], "exit", 5))
		ft_exit(data, cmd);
	else
		return (0);
	return (1);
}

void	executor(t_data *data)
{
	t_exec	ex;
	char	**cmd;
	int		is_builtin;

	ex = init_executor(data);
	data->exec = ex;
	if (ex.nbr_process == 1)
	{
		cmd = create_cmd_array(data->tokens);
		is_builtin = execute_builtin(data, cmd);
		free_matrix(cmd);
		if (is_builtin)
		{
			fflush(stdout);
			return ;
		}
	}
	create_child_process(data, &ex);
	close_all_fds(ex.fds, ex.nbr_process -1);
	wait_for_children(&ex, data);
	cleanup_executor(&ex);
	fflush(stdout);
}
