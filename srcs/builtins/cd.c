/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:25:13 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 09:50:28 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_home(char **envp)
{
	int		i;
	char	*home;

	home = NULL;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "HOME=", 5))
		{
			home = ft_strdup(envp[i] + 5);
			break ;
		}
		i++;
	}
	return (home);
}

static int	handle_home_dir(t_data *ms, char ***envp)
{
	char	*home_dir;

	home_dir = get_home(*envp);
	if (!home_dir)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		ms->error = 1;
		return (0);
	}
	set_dir(ms, home_dir, envp);
	free(home_dir);
	return (1);
}

void	ft_cd(t_data *ms, char **cmd, char ***envp)
{
	if (!cmd)
	{
		ms->error = 1;
		return ;
	}
	if (cmd[1] && !cmd[2])
		set_dir(ms, cmd[1], envp);
	else if (!cmd[1])
	{
		if (!handle_home_dir(ms, envp))
			return ;
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		ms->error = 1;
	}
}
