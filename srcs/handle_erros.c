/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_erros.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:06:25 by gabastos          #+#    #+#             */
/*   Updated: 2025/02/24 12:08:07 by gcosta-m         ###   ########.fr       */
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
