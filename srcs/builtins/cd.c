/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 10:57:01 by gabastos          #+#    #+#             */
/*   Updated: 2025/02/26 11:16:01 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_dir(t_mini *mini, char *dir, char ***envp)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = getcwd(NULL, 0);
	if (chdir(dir) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(dir, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		mini->exit = 1;
	}
	else
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			handle_erros("Error: getcwd", 0, NULL);
		*envp = update_envp(*envp, "OLDPWD", oldpwd);
		*envp = update_envp(*envp, "PWD", pwd);
		free(pwd);
	}
	free(oldpwd);
}

char	*get_home(char **envp)
{
	int		i;
	char	*home;

	i = 0;
	home = NULL;
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

void	ft_cd(t_mini *ms, char **cmd, char ***envp)
{
	if (cmd[1] && !cmd[2])
		set_dir (ms, cmd[1], envp);
	else if (!cmd[1])
		set_dir (ms, get_home (*envp), envp);
	else
	{
		ft_putstr_fd ("-" PROMPT_MSG ": cd: " INVALID_USAGE "\n", 2);
		ms -> error = 42;
	}
}
