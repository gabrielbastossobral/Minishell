/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:25:58 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 09:25:59 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(t_data *ms, char **cmd, char **envp)
{
	size_t	i;

	if (!cmd[1])
	{
		i = 0;
		while (envp && envp[i] && ft_strncmp (envp[i], "PWD=", 4))
			i++;
		if (envp && envp[i])
			ft_printf ("%s\n", envp[i] + 4);
	}
	else
	{
		ft_putstr_fd ("-" PROMPT_MSG ": pwd: " INVALID_USAGE "\n", 2);
		ms -> error = 42;
	}
}
