/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:16 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 10:16:18 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	insert_token(t_token **tokens, char *value)
{
	t_token	*new;

	new = create_token(value);
	if (!new)
		return ;
	add_to_list(tokens, new);
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
	split = gc_malloc(2 * sizeof(char *));
	if (!split)
		return (NULL);
	split_line(line, &split, &j);
	split[j] = NULL;
	return (split);
}

void	type_token(t_token **head)
{
	t_token	*token;
	int		type;

	token = *head;
	while (token)
	{
		if (!ft_strncmp(token->value, "|", 1) && ft_strlen(token->value) == 1)
			token->type = PIPE;
		else
		{
			type = check_redirection(token);
			if (type)
				token->type = type;
			else
				token->type = check_command_type(token);
		}
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
		return (1);
	}
	i = -1;
	while (cmdlist[++i])
	{
		j = -1;
		cmd = split_line_arg(cmdlist[i]);
		while (cmd[++j])
			insert_token(head, cmd[j]);
	}
	if (i > 0)
		check_pipe(cmdlist[--i], head);
	type_token(head);
	return (0);
}
