/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:29:27 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/11 13:59:09 by gcosta-m         ###   ########.fr       */
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

void	child_process(t_data *data, int pipe_index)
{
	char	**cmd;
	t_token	*token_ptr;
	int		i;

	i = -1;
	token_ptr = data->exec.tmp;
	while (++i < pipe_index && token_ptr)
	{
		while (token_ptr && token_ptr->type != PIPE)
			token_ptr = token_ptr->next;
		if (token_ptr && token_ptr->type == PIPE)
			token_ptr = token_ptr->next;
	}
	ignore_signals_in_child();
	setup_redirections(data, pipe_index);
	close_all_fds(data->exec.fds, data->exec.nbr_process - 1);
	if (!setup_redirections_for_token(data->exec.tmp))
		exit(1);
	cmd = create_cmd_array(token_ptr);
	if (!cmd || ! cmd[0])
		exit(127);
	if (token_ptr && token_ptr->type == BUILDIN && execute_builtin(data, cmd))
		exit(data->exit_error);
	execute_external(data, cmd);
	exit(127);
}

// static int	count_tokens_until_pipe(t_token *tokens)
// {
// 	t_token	*tmp;
// 	int		count;

// 	count = 0;
// 	tmp = tokens;
// 	while (tmp && tmp->type != PIPE)
// 	{
// 		count++;
// 		tmp = tmp->next;
// 	}
// 	return (count);
// }

char	**create_cmd_array(t_token *tokens)
{
	t_token	*tmp;
	char	**cmd;
	int		count;
	int		i;
	int		skip_next;

	count = 0;
	i = 0;
	skip_next = 0;
	tmp = tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (skip_next)
			skip_next = 0;
		else if (tmp->type == REDIR_OUT || tmp->type == REDIR_IN
			|| tmp->type == APPEND || tmp->type == HEREDOC)
			skip_next = 1;
		else
			count++;
		tmp = tmp->next;
	}
	cmd = gc_malloc(sizeof(char *) * (count + 1));
	if (!cmd)
	{
        ft_putstr_fd("minishell: erro ao alocar memória para comando\n", 2);
        return (NULL);
    }
	gc_add(cmd);
	tmp = tokens;
	skip_next = 0;
	while (tmp && tmp->type != PIPE)
	{
		if (skip_next)
			skip_next = 0;
		else if (tmp->type == REDIR_OUT || tmp->type == REDIR_IN
			|| tmp->type == APPEND || tmp->type == HEREDOC)
			skip_next = 1;
		else
		{
			cmd[i] = ft_strdup(tmp->value);
			if (!cmd[i])
			{
                ft_putstr_fd("minishell: erro ao duplicar valor\n", 2);
                return (NULL);
            }
			gc_add(cmd[i]);
			i++;
		}
		tmp = tmp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}
