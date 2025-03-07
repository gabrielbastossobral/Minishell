/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielsobral <gabrielsobral@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:46:20 by gabrielsobr       #+#    #+#             */
/*   Updated: 2025/03/06 21:53:32 by gabrielsobr      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	remove_var(char **envp, char *var)
{
	size_t	i;

	i = 0;
	while (envp && envp[i] && ft_strncmp(envp[i], var, ft_strlen(var)))
		i++;
	if (envp && envp[i])
	{
		free(envp[i]);
		while (envp[i])
		{
			envp[i] = envp[i + 1];
			i++;
		}
	}
}

void	ft_unset(t_data *ms, char **cmd, char ***envp)
{
	size_t	i;

	i = 1;
	while (cmd[i])
	{
		if (ft_strlen(cmd[i]) > 0)
			remove_var(*envp, cmd[i]);
		i++;
	}
	ms->error = 0;
}
