#include "../includes/minishell.h"

int prepare_heredoc_pipe(int *pipefd, int *stdin_copy)
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

int process_heredoc_line(char *line, char *delimiter, int pipefd)
{
	if (!line)
		return (0);
	if (!(ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0))
	{
		free(line);
		return (0);
	}
	ft_putstr_fd(line, pipefd);
	ft_putstr_fd("\n", pipefd);
	free(line);
	return (1);
}

void cleanup_heredoc(int *pipefd, int stdin_copy)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(stdin_copy);
	g_heredoc_status = 0;
	setup_signals();
}
