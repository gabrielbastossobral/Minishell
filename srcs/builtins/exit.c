/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielsobral <gabrielsobral@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:25:47 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/26 15:09:24 by gabrielsobr      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str)
	{
		if (!(ft_isdigit(str[i]) || str[i] == '+'))
			return (0);
		i++;
	}
	return (1);
}

static int	check_for_zeros(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] != '0')
			return (1);
	}
	return (0);
}

void	ft_exit(t_data *ms, char **cmd)
{
	ft_putstr_fd("exit\n", 2);
	if (!cmd[1])
	{
		exit(ms->error);
	}
	if (cmd[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		ms->error = 1;
		return ;
	}
	if (is_valid_number(cmd[1]))
	{
		if (check_for_zeros(cmd[1]))
			exit(ft_atoi(cmd[1]) % 256);
		else
			exit(0);
	}
	else
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
}
