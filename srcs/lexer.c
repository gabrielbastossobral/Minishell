/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:09:37 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 10:20:58 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*filler(char *input, int pos)
{
	char	*new;
	int		i;
	int		j;

	new = ft_calloc(ft_strlen(input) + 3, sizeof(char));
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
	handle_erros(NULL, 0, input);
	return (new);
}

char	**lexer(char *input)
{
	int		i;
	int		quotes;
	char	*temp;
	char	**ret;

	i = -1;
	quotes = 0;
	temp = ft_strdup(input);
	while (temp && temp[++i])
	{
		if ((temp[i] == '|' || temp[i] == '>' || temp[i] == '<' ) && !quotes)
		{
			temp = filler(temp, i);
			i = i + 2;
		}
		else if (temp[i] == '\"' || temp[i] == '\'')
			quotes = check_quotes(temp[i], quotes);
	}
	if (quotes)
		handle_erros(NULL, 0, temp);
	ret = ft_split(temp, ' ');
	handle_erros(NULL, 0, temp);
	return (ret);
}
