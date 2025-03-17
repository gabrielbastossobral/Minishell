/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:16:37 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/03/17 10:16:38 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *value)
{
	t_token	*new;
	char	*cleaned_value;

	new = gc_malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->raw_value = ft_strdup(value);
	gc_add(new->raw_value);
	detect_quote_type(value, new);
	cleaned_value = remove_quotes(value);
	new->value = cleaned_value;
	new->type = 0;
	new->next = NULL;
	new->prev = NULL;
	gc_add(new);
	return (new);
}

void	add_to_list(t_token **tokens, t_token *new)
{
	t_token	*last;

	if (!*tokens)
	{
		*tokens = new;
		return ;
	}
	last = *tokens;
	while (last->next)
		last = last->next;
	new->prev = last;
	last->next = new;
}

int	check_redirection(t_token *token)
{
	if (!ft_strncmp(token->value, ">>", 2) && ft_strlen(token->value) == 2)
		return (APPEND);
	else if (!ft_strncmp(token->value, ">", 1) && ft_strlen(token->value) == 1)
		return (REDIR_OUT);
	else if (!ft_strncmp(token->value, "<", 1) && ft_strlen(token->value) == 1)
		return (REDIR_IN);
	else if (!ft_strncmp(token->value, "<<", 2) && ft_strlen(token->value) == 2)
		return (HEREDOC);
	return (0);
}

int	check_command_type(t_token *token)
{
	if (is_builtin(token->value))
		return (BUILDIN);
	else if (!token->prev || token->prev->type == PIPE)
		return (EXECVE);
	else if (token->prev->type == REDIR_OUT || token->prev->type == REDIR_IN
		|| token->prev->type == APPEND || token->prev->type == HEREDOC)
		return (ARG_FILE);
	return (ARG);
}