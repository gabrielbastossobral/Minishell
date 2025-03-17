/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:16:29 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 10:16:52 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	wait_for_children(t_exec *ex, t_data *data)
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

void	create_child_process(t_data *data, t_exec *ex)
{
	int	i;

	ex->tmp = data->tokens;
	i = -1;
	ex->pid = gc_malloc(sizeof(pid_t) * ex->nbr_process);
	if (!ex->pid)
	{
		ft_putstr_fd("Error: malloc failed\n", 2);
		return ;
	}
	gc_add(ex->pid);
	while (++i < ex->nbr_process)
	{
		ex->pid[i] = fork();
		if (ex->pid[i] == 0)
			child_process(data, i);
		advance_to_next_cmd(ex);
	}
}
