#ifndef MINISHELL_H
# define MINISHELL_H

# include "readline/readline.h"
# include "../libs/libft/includes/not_in_standard_includes/ft_printf_bonus.h"
# include "../libs/libft/includes/not_in_standard_includes/not_in_standard.h"
# include "../libs/libft/includes/ft_stdlib.h"
# include "../libs/libft/includes/ft_string_functions.h"
# include "../libs/libft/includes/ft_ctype.h"
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

typedef struct s_mini
{
	char	*input;
	char	*prompt;
	char	**envp;
	t_token	*token;
	char	error;
	char	exit;
}	t_mini;

typedef struct sigaction	t_sig;

typedef struct s_data
{
	t_token *tokens;
	t_sig act;
	int exit_error;
	char **envp;
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
int		check_quotes(char c, int quotes);
void	free_matrix(char **matrix);
int		ft_isspace(char c);
char 	**copy_envp(char **envp);

// LEXER
char	**lexer(char *input);

// PARSER
void	insert_token(t_token **head, char *value);
int		is_builtin(char *value);
char	**split_line_arg(char *line);
void	type_token(t_token **head);
int		parser(t_token **head, char *str);

//PARSER UTILS
void	split_line(char *line, char ***split, int *j);
void	check_pipe(char *line, t_token **head);
void	*ft_realloc(void *ptr, size_t size);

//SYNTAX CHECKER
int		syntax_checker(t_data *data);

// EXPAND
void	expand(t_data *data);
char	*expand_var(char *str, char **envp);

// EXPAND VAR UTILS
void	process_quotes(char c, int *quotes);
char	*extract_var_name(char *str, int *i);
char	*append_char_to_result(char *result, char c);
char	*get_var_value(char *var_name, char **envp);
char	*append_var_value(char *result, char *var_name, char **envp);

#endif