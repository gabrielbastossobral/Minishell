/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:10:10 by gabastos          #+#    #+#             */
/*   Updated: 2025/02/24 14:44:59 by gcosta-m         ###   ########.fr       */
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
