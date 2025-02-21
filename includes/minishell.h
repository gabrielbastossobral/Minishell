#ifndef MINISHELL_H
# define MINISHELL_H

# include "readline/readline.h"
# include "../libs/libft/includes/not_in_standard_includes/ft_printf_bonus.h"
# include <fcntl.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <unistd.h>

// STRUCTS
typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

// TYPE TOKENS
enum e_type
{
	PIPE = 10,
	EXECVE = 11,
	BUILDIN = 12,
	ARG = 13,
	ARG_FILE = 14,
	REDIRECT = 15,
	HEREDOC = 16
};

// COLORS

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define RESET "\033[0m"

// HANDLE ERROS

void	handle_erros(char *msg, int stage);

#endif