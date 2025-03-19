/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:47:17 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/03/19 09:47:19 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_last_delimiter(char *delimiter, char **last_delimiter,
		int *already_processed)
{
	int	result;

	if (*already_processed && *last_delimiter)
	{
		if (ft_strlen(*last_delimiter) == ft_strlen(delimiter)
			&& ft_strncmp(*last_delimiter, delimiter,
				ft_strlen(delimiter)) == 0)
			result = 1;
		else
			result = 0;
		gc_free(*last_delimiter);
		*last_delimiter = NULL;
		*already_processed = 0;
		return (result);
	}
	if (*last_delimiter)
		gc_free(*last_delimiter);
	*last_delimiter = NULL;
	*already_processed = 0;
	return (0);
}

int	handle_heredoc_interrupt(int stdin_copy, int *pipefd)
{
	dup2(stdin_copy, STDIN_FILENO);
	safe_close(stdin_copy, pipefd[0], pipefd[1]);
	setup_signals();
	return (0);
}

int	process_heredoc_input(char *delimiter, int *pipefd)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		fflush(stdout);
		line = readline("");
		if (line)
			gc_add(line);
		if (!line && g_heredoc_status == 0)
			return (0);
		if (!line || !process_heredoc_line(line, delimiter, pipefd[1]))
			break ;
	}
	return (1);
}
