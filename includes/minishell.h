#ifndef MINISHELL_H
# define MINISHELL_H

# include "readline/readline.h"
# include "../libs/libft/includes/not_in_standard_includes/ft_printf_bonus.h"
# include "../libs/libft/includes/not_in_standard_includes/not_in_standard.h"
# include "../libs/libft/includes/ft_stdlib.h"
# include "../libs/libft/includes/ft_string_functions.h"
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

typedef struct sigaction	t_sig;

typedef struct s_args
{
	char *arg;
	struct s_args *next;
}	t_args;

typedef struct s_cmd
{
	char *cmd;
	char *flags;
	t_args *args;
	struct s_cmd *next;
	int fd_input;
	int fd_output;
}			t_cmd;

typedef struct s_vars
{
	int is_env;
	char *var;
	char *value;
	struct vars *next;
}			t_vars;

typedef struct s_data
{
	t_cmd *cmds;
	t_token *tokens;
	t_vars *vars;
	char **envp;
	t_sig act;
}	t_data;

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

void	handle_erros(char *msg, int stage, void *ptr);

// UTILS
char	**lexer(char *input);
int		check_quotes(char c, int quotes);
void	check_pipe(char *line, t_token **head);
void	*ft_realloc(void *ptr, size_t size);


// PARSER
int		parser(t_token **head, char *str);
void	insert_token(t_token **head, char *value);
int		is_builtin(char *value);

#endif