/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:10 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 10:46:59 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	detect_quote_type(char *str, t_token *token)
{
	int	i;

	i = 0;
	token->quote_type = 0;
	while (str[i] && !token->quote_type)
	{
		if (str[i] == '\'' || str[i] == '\"')
			token->quote_type = str[i];
		i++;
	}
}

static int	quotes(char c, char *quote_char)
{
	if (*quote_char == 0)
		*quote_char = c;
	else if (*quote_char == c)
		*quote_char = 0;
	return (1);
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;
	char	quote_char;

	i = 0;
	j = 0;
	quote_char = 0;
	result = gc_malloc(ft_strlen(str) + 1);
	while (str[i])
	{
		if (str[i] == '\\' && (str[i + 1] == '\"' || str[i + 1] == '\''))
		{
			i++;
			result[j++] = str[i++];
		}
		else if ((str[i] == '\'' || str[i] == '\"') && (quote_char == 0
				|| quote_char == str[i]))
			i += quotes(str[i], &quote_char);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

void	check_pipe(char *line, t_token **head)
{
	char	*input;

	input = NULL;
	if (!ft_strncmp(line, "|", 2))
	{
		input = readline("> ");
		if (!input)
			return ;
		gc_add(input);
		if (input && !input[0])
			return ;
		parser(head, input);
	}
}

static void	add_substring(char ***split, char *line, int len, int *j)
{
	char	**new_split;
	int		i;

	(*split)[*j] = ft_substr(line, 0, len);
	gc_add((*split)[*j]);
	(*j)++;
	new_split = gc_malloc((*j + 1) * sizeof(char *));
	if (!new_split)
		return ;
	i = -1;
	while (++i < *j)
		new_split[i] = (*split)[i];
	new_split[*j] = NULL;
	*split = new_split;
}

void	split_line(char *line, char ***split, int *j)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
			quotes = check_quotes(line[i], quotes);
		if (line[i] == ' ' && !quotes)
		{
			if (i > 0)
				add_substring(split, line, i, j);
			line += i + 1;
			i = 0;
		}
		else
			i++;
	}
	if (*line)
		add_substring(split, line, i, j);
}
