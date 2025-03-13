/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielsobral <gabrielsobral@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:10 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/12 12:04:43 by gabrielsobr      ###   ########.fr       */
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
		{
			if (quote_char == 0)
				quote_char = str[i];
			else
				quote_char = 0;
			i++;
		}
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

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (!ptr)
		return (gc_malloc(size));
	new_ptr = gc_malloc(size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	return (new_ptr);
}

static void	add_substring(char ***split, char *line, int len, int *j)
{
	(*split)[*j] = ft_substr(line, 0, len);
	gc_add((*split)[*j]);
	(*j)++;
	*split = ft_realloc(*split, (*j + 2) * sizeof(char *));
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
