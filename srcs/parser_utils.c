/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:10 by gabastos          #+#    #+#             */
/*   Updated: 2025/02/26 09:55:46 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_pipe(char *line, t_token **head)
{
	char	*input;

	input = NULL;
	if (!ft_strncmp(line, "|", 2))
	{
		input = readline("> ");
		if (input && !input[0])
		{
			handle_erros(NULL, 0, input);
			return ;
		}
		parser(head, input);
		handle_erros(NULL, 0, input);
	}
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (!ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

static void	add_substring(char ***split, char *line, int len, int *j)
{
	(*split)[*j] = ft_substr(line, 0, len);
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
