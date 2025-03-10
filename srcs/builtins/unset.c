/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:26:03 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 09:26:04 by gabastos         ###   ########.fr       */
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
