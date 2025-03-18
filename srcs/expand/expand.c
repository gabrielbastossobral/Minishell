/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:26:20 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/18 10:22:55 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_var(char *str, char **envp)
{
	int		i;
	char	*result;
	char	*var_name;
	int		quotes;

	i = 0;
	result = ft_strdup("");
	gc_add(result);
	quotes = 0;
	while (str[i])
	{
		process_quotes(str[i], &quotes);
		if (str[i] == '$' && quotes != 2)
		{
			var_name = extract_var_name(str, &i);
			if (!*var_name)
				result = append_char_to_result(result, '$');
			else
				result = append_var_value(result, var_name, envp);
		}
		else
			result = append_char_to_result(result, str[i++]);
	}
	return (result);
}

static int	handle_quotes(char c, int *in_sq, int *in_dq)
{
	if (c == '\'' && !(*in_dq))
	{
		*in_sq = !(*in_sq);
		return (1);
	}
	else if (c == '\"' && !(*in_sq))
	{
		*in_dq = !(*in_dq);
		return (1);
	}
	return (0);
}

static char	*process_expansion(char *result, char *raw, int *i, char **envp)
{
	char	*var_name;

	var_name = extract_var_name(raw, i);
	if (!*var_name)
		result = append_char_to_result(result, '$');
	else
		result = append_var_value(result, var_name, envp);
	return (result);
}

static char	*smart_expand(char *raw, char **envp)
{
	int		i;
	int		in_sq;
	int		in_dq;
	char	*result;

	i = 0;
	in_sq = 0;
	in_dq = 0;
	result = ft_strdup("");
	gc_add(result);
	while (raw[i])
	{
		if (handle_quotes(raw[i], &in_sq, &in_dq))
			i++;
		else if (raw[i] == '$' && !in_sq)
			result = process_expansion(result, raw, &i, envp);
		else
		{
			result = append_char_to_result(result, raw[i]);
			i++;
		}
	}
	return (result);
}

void	expand(t_data *data)
{
	t_token	*token;
	char	*expanded_value;

	token = data->tokens;
	while (token)
	{
		if (ft_strchr(token->raw_value, '$'))
		{
			expanded_value = smart_expand(token->raw_value, data->envp);
			token->value = expanded_value;
		}
		token = token->next;
	}
}
