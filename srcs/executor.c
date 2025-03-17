/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:36:31 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 10:17:21 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	restore_io(int stdin_fd, int stdout_fd)
{
	if (stdin_fd != -1)
	{
		dup2(stdin_fd, STDIN_FILENO);
		close(stdin_fd);
	}
	if (stdout_fd != -1)
	{
		dup2(stdout_fd, STDOUT_FILENO);
		close(stdout_fd);
	}
}

static int	handle_single_command_redirections(t_data *data, int *stdin_fd,
		int *stdout_fd)
{
	*stdin_fd = dup(STDIN_FILENO);
	*stdout_fd = dup(STDOUT_FILENO);
	if (!setup_redirections_for_token(data->tokens))
	{
		data->exit_error = 1;
		restore_io(*stdin_fd, *stdout_fd);
		return (0);
	}
	return (1);
}

static int	execute_single_command(t_data *data, int stdin_fd, int stdout_fd)
{
	char	**cmd;
	int		is_builtin;

	cmd = create_cmd_array(data->tokens);
	is_builtin = 0;
	if (cmd && cmd[0])
		is_builtin = execute_builtin(data, cmd);
	restore_io(stdin_fd, stdout_fd);
	return (is_builtin);
}

void	executor(t_data *data)
{
	t_exec	ex;
	int		saved_stdin;
	int		saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
	ex = init_executor(data);
	data->exec = ex;
	if (ex.nbr_process == 1)
	{
		if (!handle_single_command_redirections(data, &saved_stdin,
				&saved_stdout))
			return ;
		if (execute_single_command(data, saved_stdin, saved_stdout))
		{
			fflush(stdout);
			return ;
		}
	}
	create_child_process(data, &ex);
	close_all_fds(ex.fds, ex.nbr_process - 1);
	wait_for_children(&ex, data);
	fflush(stdout);
}
