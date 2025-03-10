/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:09:47 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 09:25:02 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init(t_data *data, char **envp)
{
	data->tokens = NULL;
	data->envp = copy_envp(envp);
	data->exit_error = 0;
}

char	*get_input()
{
	char	*line;

	rl_on_new_line();
	line = readline(GREEN"☯️💰🤡$MINI$HELL_DE_VILÃO$🤡💰☯️ $ "RESET);
	if (!line)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(0);
	}
	if (line && *line)
		add_history(line);
	return (line);
}

static void	clear_data(t_data *data)
{
	t_token	*current;
	t_token	*next;

	current = data->tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	data->tokens = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_data	data;

	if (argc != 1 || argv[1])
		handle_erros("Usage: ./minishell", 0, NULL);
	init(&data, envp);
	setup_signals();
	while (1)
	{
		line = get_input();
		if (!parser(&data.tokens, line) && !syntax_checker(&data))
		{
			expand(&data);
			executor(&data);
			rl_on_new_line();
		}
		clear_data(&data);
		free(line);
	}
	return (0);
}
