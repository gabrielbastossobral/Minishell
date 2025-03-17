/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:00:05 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 10:05:40 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_command_args(t_token *tokens)
{
	t_token	*tmp;
	int		count;
	int		skip_next;

	count = 0;
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
	return (count);
}

static int	is_redirection_token(int type)
{
	return (type == REDIR_OUT || type == REDIR_IN || type == APPEND
		|| type == HEREDOC);
}

static int	process_token(t_token *tmp, char **cmd, int *i)
{
	if (!is_redirection_token(tmp->type))
	{
		cmd[*i] = ft_strdup(tmp->value);
		if (!cmd[*i])
		{
			ft_putstr_fd("minishell: erro ao duplicar valor\n", 2);
			return (0);
		}
		gc_add(cmd[(*i)++]);
	}
	return (1);
}

static int	fill_cmd_array(t_token *tokens, char **cmd)
{
	t_token	*tmp;
	int		i;
	int		skip_next;

	i = 0;
	skip_next = 0;
	tmp = tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (skip_next)
			skip_next = 0;
		else if (is_redirection_token(tmp->type))
			skip_next = 1;
		else if (!process_token(tmp, cmd, &i))
			return (0);
		tmp = tmp->next;
	}
	cmd[i] = NULL;
	return (1);
}

char	**create_cmd_array(t_token *tokens)
{
	char	**cmd;
	int		count;

	if (!tokens)
		return (NULL);
	count = count_command_args(tokens);
	cmd = gc_malloc(sizeof(char *) * (count + 1));
	if (!cmd)
	{
		ft_putstr_fd("minishell: erro ao alocar memória para comando\n", 2);
		return (NULL);
	}
	gc_add(cmd);
	if (!fill_cmd_array(tokens, cmd))
		return (NULL);
	return (cmd);
}
