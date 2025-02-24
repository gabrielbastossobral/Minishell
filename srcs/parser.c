/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:16 by gabastos          #+#    #+#             */
/*   Updated: 2025/02/24 15:07:42 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	insert_token(t_token **tokens, char *value)
{
	t_token	*new;
	t_token	*last;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		handle_erros("Error: malloc", 0, NULL);
	new->value = value;
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

static int	is_builtin(char *value)
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

static char	**split_line_arg(char *line)
{
	int		i;
	int		quotes;
	int		j;
	char	**split;

	i = 0;
	quotes = 0;
	j = 0;
	split = ft_calloc(2, sizeof(char *));
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
			quotes = check_quotes(line[i], quotes);
		if (line[i] == ' ' && !quotes)
		{
			split[j] = ft_substr(line, 0, i);
			line += i + 1;
			i = 0;
			j++;
			split = ft_realloc(split, (j + 2) * sizeof(char *));
		}
		i++;
	}
	split[j] = ft_strdup(line);
	return (split);
}

static void	type_token(t_token **head)
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
