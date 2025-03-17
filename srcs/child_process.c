/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:29:27 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 10:03:20 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_pipe_fds(int **fds, int pipe_count)
{
	int	i;

	if (!fds)
		return ;
	i = -1;
	while (++i < pipe_count)
		free(fds[i]);
	free(fds);
}

static void	setup_redirections(t_data *data, int pipe_index)
{
	int	pipe_count;

	pipe_count = data->exec.nbr_process - 1;
	if (pipe_index > 0)
		dup2(data->exec.fds[pipe_index - 1][0], STDIN_FILENO);
	if (pipe_index < pipe_count)
		dup2(data->exec.fds[pipe_index][1], STDOUT_FILENO);
}

static t_token	*find_command_token(t_token *start, int pipe_index)
{
	t_token	*token_ptr;
	int		i;

	i = -1;
	token_ptr = start;
	while (++i < pipe_index && token_ptr)
	{
		while (token_ptr && token_ptr->type != PIPE)
			token_ptr = token_ptr->next;
		if (token_ptr && token_ptr->type == PIPE)
			token_ptr = token_ptr->next;
	}
	return (token_ptr);
}

static void	execute_command(t_data *data, t_token *token_ptr, int pipe_index)
{
	char	**cmd;

	cmd = create_cmd_array(token_ptr);
	if (!cmd || !cmd[0])
		exit(127);
	if (pipe_index > 0 || data->exec.nbr_process > 1)
	{
		if (token_ptr && token_ptr->type == BUILDIN && execute_builtin(data,
				cmd))
			exit(data->exit_error);
	}
	execute_external(data, cmd);
	exit(127);
}

void	child_process(t_data *data, int pipe_index)
{
	t_token	*token_ptr;

	token_ptr = find_command_token(data->exec.tmp, pipe_index);
	ignore_signals_in_child();
	setup_redirections(data, pipe_index);
	close_all_fds(data->exec.fds, data->exec.nbr_process - 1);
	if (!setup_redirections_for_token(data->exec.tmp))
		exit(1);
	execute_command(data, token_ptr, pipe_index);
}
