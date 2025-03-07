/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielsobral <gabrielsobral@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:34:16 by gabrielsobr       #+#    #+#             */
/*   Updated: 2025/03/06 21:43:30 by gabrielsobr      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(t_mini *ms, char **cmd, char **envp)
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
