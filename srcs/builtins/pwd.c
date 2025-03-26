/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielsobral <gabrielsobral@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:25:58 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/26 15:05:27 by gabrielsobr      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(t_data *ms, char **cmd, char **envp)
{
	char	cwd[4096];

	(void)envp;
	if (!cmd[1])
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			ft_printf("%s\n", cwd);
		else
			perror("pwd");
	}
	else
	{
		ft_putstr_fd("-" PROMPT_MSG ": pwd: " INVALID_USAGE "\n", 2);
		ms->error = 42;
	}
}
