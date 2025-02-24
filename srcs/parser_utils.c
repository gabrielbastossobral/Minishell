/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:10 by gabastos          #+#    #+#             */
/*   Updated: 2025/02/24 10:10:10 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
