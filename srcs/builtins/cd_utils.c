/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:48:56 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 09:50:00 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	change_directory(t_data *ms, char *dir, char **oldpwd, char **pwd)
{
	*oldpwd = getcwd(NULL, 0);
	if (!*oldpwd)
	{
		perror(PROMPT_MSG ": cd");
		ms->error = 1;
		return (0);
	}
	if (chdir(dir) != 0)
	{
		perror(PROMPT_MSG ": cd");
		ms->error = 1;
		free(*oldpwd);
		return (0);
	}
	*pwd = getcwd(NULL, 0);
	if (!*pwd)
	{
		perror(PROMPT_MSG ": cd");
		ms->error = 1;
		free(*oldpwd);
		return (0);
	}
	return (1);
}

static void	update_pwd_vars(t_data *ms, char *oldpwd, char *pwd, char ***envp)
{
	char	**var;

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

void	set_dir(t_data *ms, char *dir, char ***envp)
{
	char	*oldpwd;
	char	*pwd;

	if (!change_directory(ms, dir, &oldpwd, &pwd))
		return ;
	update_pwd_vars(ms, oldpwd, pwd, envp);
}
