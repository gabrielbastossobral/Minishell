/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_erros.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:49:00 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 09:50:01 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_erros(char *msg, int stage, void *ptr)
{
	int		i;
	char	**arr;

	if (ptr)
	{
		if (stage == 1)
		{
			arr = (char **)ptr;
			i = -1;
			while (arr[++i])
				free(arr[i]);
			free(arr);
		}
		else
		{
			free(ptr);
		}
	}
	if (msg)
		ft_printf("%s%s%s\n", RED, msg, RESET);
}
