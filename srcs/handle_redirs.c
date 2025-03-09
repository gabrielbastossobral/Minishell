#include "../includes/minishell.h"

int handle_redir_out(char *filename)
{
    int fd;

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

int handle_redir_in(char *filename)
{
    int fd;

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

int handle_redir_append(char *filename)
{
    int fd;

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

int handle_heredoc(char *delimiter)
{
    int pipefd[2];
    char *line;

    if (pipe(pipefd) == -1)
        return (0);
    while (1)
    {
        ft_putstr_fd("> ", STDOUT_FILENO);
        line = readline("");
        if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
        {
            free(line);
            break;
        }
        ft_putstr_fd(line, pipefd[1]);
        ft_putstr_fd("\n", pipefd[1]);
        free(line);
    }
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    return (1);
}

int setup_redirections_for_token(t_token *tokens)
{
    t_token *token;

    token = tokens;
    while (token && token->type != PIPE)
    {
        if (token->type == REDIR_OUT || token->type == REDIR_IN ||
            token->type == APPEND || token->type == HEREDOC)
        {
            if (!token->next || (token->next->type != ARG && token->next->type != ARG_FILE))
            {
                ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
                return (0);
            }
            if(token->type == REDIR_OUT && !handle_redir_out(token->next->value))
                return (0);
            else if (token->type == REDIR_IN && !handle_redir_in(token->next->value))
                return (0);
            else if (token->type == APPEND && !handle_redir_append(token->next->value))
                return (0);
            else if (token->type == HEREDOC && !handle_heredoc(token->next->value))
                return (0);
        }
        token = token->next;
    }
    return (1);
}
