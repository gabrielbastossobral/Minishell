/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:25:13 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/11 11:42:13 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	set_dir(t_data *ms, char *dir, char ***envp)
{
	char	*oldpwd;
	char	*pwd;
	char	**var;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror(PROMPT_MSG ": cd");
		ms->error = 1;
		return ;
	}
	if (chdir(dir) != 0)
	{
		perror(PROMPT_MSG ": cd");
		ms->error = 1;
		free(oldpwd);
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror(PROMPT_MSG ": cd");
		ms->error = 1;
		free(oldpwd);
		return ;
	}
	var = ft_calloc(3, sizeof(char *));
	if (!var)
	{
		free(oldpwd);
		free(pwd);
		ms->error = 1;
		return ;
	}
	var[0] = ft_strdup("export");
	var[1] = ft_strjoin("OLDPWD=", oldpwd);
	ft_export(ms, var, envp);
	free(var[1]);
	var[1] = ft_strjoin("PWD=", pwd);
	ft_export(ms, var, envp);
	free(var[1]);
	free(var[0]);
	free(var);
	free(oldpwd);
	free(pwd);
	ms->error = 0;
}

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

void	ft_cd(t_data *ms, char **cmd, char ***envp)
{
	char	*home_dir;

	home_dir = NULL;
	if (!cmd)
	{
		ms->error = 1;
		return ;
	}
	if (cmd[1] && !cmd[2])
		set_dir(ms, cmd[1], envp);
	else if (!cmd[1])
	{
		home_dir = get_home(*envp);
		if (!home_dir)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			ms->error = 1;
			return ;
		}
		set_dir(ms, home_dir, envp);
		free(home_dir);
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		ms->error = 1;
	}
}
