/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:09:47 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 11:29:12 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			g_heredoc_status = 1;

static void	init(t_data *data, char **envp)
{
	data->tokens = NULL;
	data->envp = copy_envp(envp);
	data->exit_error = 0;
	data->error = 0;
}

char	*get_input(void)
{
	char	*line;

	rl_on_new_line();
	line = readline(GREEN "☯️💰🤡$MINI$HELL_DE_VILÃO$🤡💰☯️ $ " RESET);
	if (!line)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		gc_exit();
		exit(0);
	}
	if (line && *line)
		add_history(line);
	gc_add(line);
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
		gc_free(current->value);
		gc_free(current);
		current = next;
	}
	data->tokens = NULL;
}

static void	process_command(t_data *data, char *line)
{
	if (!parser(&data->tokens, line) && !syntax_checker(data))
	{
		expand(data);
		executor(data);
		rl_on_new_line();
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_data	data;

	if (argc != 1 || argv[1])
	{
		ft_putstr_fd("minishell: invalid arguments\n", STDERR_FILENO);
		return (1);
	}
	init(&data, envp);
	setup_signals();
	while (1)
	{
		line = get_input();
		if (line && *line)
			process_command(&data, line);
		clear_data(&data);
	}
	gc_exit();
	return (0);
}
