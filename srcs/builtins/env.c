/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:25:41 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 09:25:43 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_env(t_data *ms, char **cmd, char ***envp)
{
	size_t	i;

	if (cmd[1])
	{
		ft_putstr_fd ("-" PROMPT_MSG ": env: " INVALID_USAGE "\n", 2);
		ms -> error = 42;
	}
	else
	{
		i = 0;
		while (envp && *envp && envp[0][i])
		{
			ft_printf ("%s\n", envp[0][i]);
			i++;
		}
		ms -> error = 0;
	}
}
