/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:17:21 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 10:34:33 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_next(t_data *data, t_token *token, int type)
{
	if (type == PIPE && token->next && token->next->type == PIPE)
	{
		ft_printf("minishell: syntax error near unexpected token %s\n",
			token->next->value);
		data->exit_error = 2;
		return (2);
	}
	else if (((type == REDIR_OUT || type == REDIR_IN || type == APPEND
				|| type == HEREDOC) && token->next == NULL)
		|| ((type == REDIR_OUT || type == REDIR_IN || type == APPEND
				|| type == HEREDOC) && (token->next->type == PIPE
				|| token->next->type == REDIR_OUT
				|| token->next->type == REDIR_IN || token->next->type == APPEND
				|| token->next->type == HEREDOC)))
	{
		ft_printf("minishell: syntax error near unexpected token %s\n",
			token->value);
		data->exit_error = 2;
		return (2);
	}
	return (0);
}

int	syntax_checker(t_data *data)
{
	t_token	*tmp;

	tmp = data->tokens;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (check_next(data, tmp, PIPE))
				return (2);
		}
		else if ((tmp->type == REDIR_OUT || tmp->type == REDIR_IN
				|| tmp->type == APPEND || tmp->type == HEREDOC))
		{
			if (check_next(data, tmp, tmp->type))
				return (2);
		}
		else if (ft_strchr(tmp->value, '\\'))
		{
			ft_printf("minishell: syntax error, token `\\' is not accepted\n");
			data->exit_error = 2;
			return (2);
		}
		tmp = tmp->next;
	}
	return (0);
}
