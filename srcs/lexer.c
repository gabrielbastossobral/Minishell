/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:09:37 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 10:36:17 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*insert_space(char *input, int pos)
{
	int		len;
	char	*new;
	int		i;
	int		j;

	len = ft_strlen(input);
	new = gc_malloc(len + 2);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (i <= pos)
		new[j++] = input[i++];
	new[j++] = ' ';
	while (i < len)
		new[j++] = input[i++];
	new[j] = '\0';
	return (new);
}

static char	*filler(char *input, int pos)
{
	char	*new;
	int		i;
	int		j;

	new = gc_malloc(ft_strlen(input) + 3);
	if (!new)
		return (NULL);
	i = -1;
	j = -1;
	while (++j < pos)
		new[j] = input[++i];
	i++;
	new[j++] = ' ';
	new[j++] = input[i++];
	if (input[i] == input[i - 1] && input[i] != '|')
		new[j++] = input[i++];
	new[j++] = ' ';
	while (input[i])
		new[j++] = input[i++];
	new[j] = '\0';
	return (new);
}

static char	**split_quoted(char *str)
{
	char	**result;
	int		i;

	result = gc_malloc(2 * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	split_line(str, &result, &i);
	result[i] = NULL;
	return (result);
}

static char	*process_input(char *temp, int *quotes)
{
	int	i;
	int	prev_quote;

	i = -1;
	while (temp && temp[++i])
	{
		if ((temp[i] == '|' || temp[i] == '>' || temp[i] == '<') && !(*quotes))
		{
			temp = filler(temp, i);
			i = i + 2;
		}
		else if (temp[i] == '\"' || temp[i] == '\'')
		{
			prev_quote = *quotes;
			*quotes = check_quotes(temp[i], *quotes);
			if (prev_quote && !(*quotes) && temp[i + 1] && (temp[i + 1] == '|'
					|| temp[i + 1] == '>' || temp[i + 1] == '<'))
			{
				temp = insert_space(temp, i);
				i++;
			}
		}
	}
	return (temp);
}

char	**lexer(char *input)
{
	char	*temp;
	char	**ret;
	int		quotes;

	quotes = 0;
	temp = ft_strdup(input);
	gc_add(temp);
	temp = process_input(temp, &quotes);
	if (quotes)
		return (NULL);
	ret = split_quoted(temp);
	return (ret);
}
