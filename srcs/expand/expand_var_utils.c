/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:42:44 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 09:46:29 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_quotes(char c, int *quotes)
{
	if (c == '\'' || c == '\"')
		*quotes = check_quotes(c, *quotes);
}

char	*extract_var_name(char *str, int *i)
{
	int		start;
	char	*var_name;

	(*i)++;
	start = *i;
	if (str[*i] == '\0' || str[*i] == ' ' || str[*i] == '\'' || str[*i] == '\"')
		return (ft_strdup(""));
	while (str[*i] && ft_isalnum(str[*i]))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	gc_add(var_name);
	return (var_name);
}

char	*append_char_to_result(char *result, char c)
{
	char	tmp[2];
	char	*new_result;

	tmp[0] = c;
	tmp[1] = '\0';
	new_result = ft_strjoin(result, tmp);
	gc_add(result);
	return (new_result);
}

char	*get_var_value(char *var_name, char **envp)
{
	int	i;
	int	var_len;

	if (!var_name || !*var_name)
		return (NULL);
	var_len = ft_strlen(var_name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, var_len) == 0
			&& envp[i][var_len] == '=')
			return (envp[i] + var_len + 1);
		i++;
	}
	return (NULL);
}

char	*append_var_value(char *result, char *var_name, char **envp)
{
	char	*var_value;
	char	*new_result;

	var_value = get_var_value(var_name, envp);
	if (var_value)
	{
		new_result = ft_strjoin(result, var_value);
		gc_add(result);
	}
	else
		new_result = result;
	return (new_result);
}
