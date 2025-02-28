/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:27 by gabastos          #+#    #+#             */
/*   Updated: 2025/02/24 11:03:25 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_quotes(char c, int quotes)
{
	if (c == '\"')
	{
		if (!quotes)
			return (1);
		else if (quotes == 1)
			return (0);
	}
	else if (c == '\'')
	{
		if (!quotes)
			return (2);
		else if (quotes == 2)
			return (0);
	}
	return (quotes);
}

void	free_matrix(char **matrix)
{
	int	i;

	i = -1;
	while (matrix[++i])
		free(matrix[i]);
	free(matrix);
}

int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

char **copy_envp(char **envp)
{
	char **new;
	int i;

	i = 0;
	while (envp[i])
		i++;
	new = ft_calloc(i + 1, sizeof(char *));
	if (!new)
		return (0);
	i = -1;
	while (envp[++i])
	{
		new[i] = ft_strdup(envp[i]);
		if (!new[i])
			free_matrix(new);
	}
	return (new);
}
