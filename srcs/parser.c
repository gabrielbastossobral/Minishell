/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:16 by gabastos          #+#    #+#             */
/*   Updated: 2025/02/25 11:17:35 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	insert_token(t_token **tokens, char *value)
{
	t_token	*new;
	t_token	*last;
	char 	*cleaned_value;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		handle_erros("Error: malloc", 0, NULL);
	detect_quote_type(value, new);
	cleaned_value = remove_quotes(value);
	new->value = cleaned_value;
	new->type = 0;
	new->next = NULL;
	new->prev = NULL;
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

int	is_builtin(char *value)
{
	if (!ft_strncmp(value, "echo", 5))
		return (1);
	if (!ft_strncmp(value, "cd", 3))
		return (1);
	if (!ft_strncmp(value, "pwd", 4))
		return (1);
	if (!ft_strncmp(value, "export", 7))
		return (1);
	if (!ft_strncmp(value, "unset", 6))
		return (1);
	if (!ft_strncmp(value, "env", 4))
		return (1);
	if (!ft_strncmp(value, "exit", 5))
		return (1);
	return (0);
}

char	**split_line_arg(char *line)
{
    char	**split;
    int		j;

    j = 0;
    split = ft_calloc(2, sizeof(char *));
    split_line(line, &split, &j);
    split[j] = NULL;
    return (split);
}

void	type_token(t_token **head)
{
	t_token	*token;

	token = *head;
	while (token)
	{
		if (!ft_strncmp(token->value, "|", ft_strlen(token->value)))
			token->type = PIPE;
		else if (!ft_strncmp(token->value, ">>", ft_strlen(token->value)) || \
		!ft_strncmp(token->value, "<", ft_strlen(token->value)))
			token->type = REDIRECT;
		else if (!ft_strncmp(token->value, "<<", ft_strlen("<<")))
			token->type = HEREDOC;
		else if (is_builtin(token->value))
			token->type = BUILDIN;
		else if (!token->prev || token->prev->type == PIPE)
			token->type = EXECVE;
		else if (token->prev->type == REDIRECT)
			token->type = ARG_FILE;
		else
			token->type = ARG;
		token = token->next;
	}
}

int	parser(t_token **head, char *str)
{
	char	**cmdlist;
	char	**cmd;
	int		i;
	int		j;

	cmdlist = lexer(str);
	if (!cmdlist)
	{
		ft_putstr_fd("-minishell: parser: unclosed quotes\n", 2);
		handle_erros(NULL, 1, cmdlist);
		return (1);
	}
	i = -1;
	while (cmdlist[++i])
	{
		j = -1;
		cmd = split_line_arg(cmdlist[i]);
		while (cmd[++j])
			insert_token(head, cmd[j]);
		handle_erros(NULL, 1, cmd);
	}
	check_pipe (cmdlist[--i], head);
	handle_erros(NULL, 1, cmdlist);
	type_token(head);
	return (0);
}
