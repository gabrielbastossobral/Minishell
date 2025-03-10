/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:50:23 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/10 15:28:16 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_redir_out(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	handle_redir_in(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

int	handle_redir_append(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	handle_heredoc(char *delimiter)
{
	int		pipefd[2];
	char	*line;
	int 	stdin_copy;

	if (!prepare_heredoc_pipe(pipefd, &stdin_copy))
		return (0);
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		fflush(stdout);
		line = readline("");
		if (!line && g_heredoc_status == 0)
		{
			dup2(stdin_copy, STDIN_FILENO);
			close(stdin_copy);
			close(pipefd[0]);
			close(pipefd[1]);
			setup_signals();
			return (0);
		}
		if (!line || !process_heredoc_line(line, delimiter, pipefd[1]))
			break ;
	}
	cleanup_heredoc(pipefd, stdin_copy);
	return (1);
}

int	setup_redirections_for_token(t_token *tokens)
{
	t_token	*token;

	token = tokens;
	while (token && token->type != PIPE)
	{
		if (token->type == REDIR_OUT || token->type == REDIR_IN
			|| token->type == APPEND || token->type == HEREDOC)
		{
			if (!token->next || (token->next->type != ARG
					&& token->next->type != ARG_FILE))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token\n",
					STDERR_FILENO);
				return (0);
			}
			if (token->type == REDIR_OUT
				&& !handle_redir_out(token->next->value))
				return (0);
			else if (token->type == REDIR_IN
				&& !handle_redir_in(token->next->value))
				return (0);
			else if (token->type == APPEND
				&& !handle_redir_append(token->next->value))
				return (0);
			else if (token->type == HEREDOC
				&& !handle_heredoc(token->next->value))
				return (0);
		}
		token = token->next;
	}
	return (1);
}
