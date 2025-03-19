/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:50:23 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/17 11:20:33 by gabastos         ###   ########.fr       */
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
    int			pipefd[2];
    int			stdin_copy;
    static char	*last_delimiter;
    static int	already_processed;
    int			result;

    if (check_last_delimiter(delimiter, &last_delimiter, &already_processed))
        return (1);
    if (!prepare_heredoc_pipe(pipefd, &stdin_copy))
        return (0);
    result = process_heredoc_input(delimiter, pipefd);
    if (!result)
        return (handle_heredoc_interrupt(stdin_copy, pipefd));
    cleanup_heredoc(pipefd, stdin_copy);
	if (last_delimiter)
		gc_free(last_delimiter);
    last_delimiter = ft_strdup(delimiter);
	gc_add(last_delimiter);
    already_processed = 1;
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
			if (!validate_redirection_token(token))
				return (0);
			if (!process_redirection(token))
				return (0);
		}
		token = token->next;
	}
	return (1);
}
