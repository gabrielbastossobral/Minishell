/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_erros.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielsobral <gabrielsobral@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:06:25 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/06 21:23:51 by gabrielsobr      ###   ########.fr       */
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
