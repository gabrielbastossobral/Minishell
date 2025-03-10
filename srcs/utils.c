/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:27 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 10:20:47 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(char *path, char **envp)
{
	int	i;
	int	path_len;

	if (!path || !envp)
		return (NULL);
	path_len = ft_strlen(path);
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], path, path_len) == 0
			&& envp[i][path_len] == '=')
			return (envp[i] + path_len + 1);
	}
	return (NULL);
}

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

char	**copy_envp(char **envp)
{
	char	**new;
	int		i;

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
