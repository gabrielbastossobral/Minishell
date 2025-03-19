/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:27:28 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/19 09:49:25 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	prepare_heredoc_pipe(int *pipefd, int *stdin_copy)
{
	*stdin_copy = dup(STDIN_FILENO);
	if (pipe(pipefd) == -1)
	{
		close(*stdin_copy);
		return (0);
	}
	setup_signals_heredoc();
	return (1);
}

int	process_heredoc_line(char *line, char *delimiter, int pipefd)
{
	if (!line)
		return (0);
	if (ft_strlen(line) == ft_strlen(delimiter) && \
		ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		return (0);
	ft_putstr_fd(line, pipefd);
	ft_putstr_fd("\n", pipefd);
	return (1);
}

void	cleanup_heredoc(int *pipefd, int stdin_copy)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(stdin_copy);
	g_heredoc_status = 0;
	setup_signals();
}

int	validate_redirection_token(t_token *token)
{
	if (!token->next || (token->next->type != ARG
			&& token->next->type != ARG_FILE))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n",
			STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	process_redirection(t_token *token)
{
	if (token->type == REDIR_OUT)
		return (handle_redir_out(token->next->value));
	else if (token->type == REDIR_IN)
		return (handle_redir_in(token->next->value));
	else if (token->type == APPEND)
		return (handle_redir_append(token->next->value));
	else if (token->type == HEREDOC)
		return (handle_heredoc(token->next->value));
	return (1);
}
