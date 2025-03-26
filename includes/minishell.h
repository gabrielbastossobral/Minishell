/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielsobral <gabrielsobral@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:38:41 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/26 15:06:45 by gabrielsobr      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/includes/ft_ctype.h"
# include "../libs/libft/includes/ft_stdlib.h"
# include "../libs/libft/includes/ft_string_functions.h"
# include "../libs/libft/includes/not_in_standard_includes/ft_gc.h"
# include "../libs/libft/includes/not_in_standard_includes/ft_printf_bonus.h"
# include "../libs/libft/includes/not_in_standard_includes/get_next_line.h"
# include "../libs/libft/includes/not_in_standard_includes/not_in_standard.h"
# include "readline/readline.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

// STRUCTS
typedef struct s_token
{
	char					*raw_value;
	char					*value;
	int						type;
	char					quote_type;
	struct s_token			*next;
	struct s_token			*prev;
}							t_token;

typedef struct s_exec
{
	pid_t					*pid;
	int						status;
	int						nbr_process;
	int						**fds;
	char					**cmd;
	t_token					*tmp;
}							t_exec;

typedef struct sigaction	t_sig;

typedef struct s_data
{
	t_token					*tokens;
	t_sig					act;
	t_exec					exec;
	int						exit_error;
	int						error;
	char					**envp;
}							t_data;

// TYPE TOKENS
enum						e_type
{
	PIPE = 10,
	EXECVE = 11,
	BUILDIN = 12,
	ARG = 13,
	ARG_FILE = 14,
	REDIR_OUT = 15,
	REDIR_IN = 16,
	APPEND = 17,
	HEREDOC = 18
};

extern int					g_heredoc_status;

// COLORS
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define RESET "\033[0m"

// PROMPT
# ifndef PROMPT_MSG
#  define PROMPT_MSG "minishell"
# endif

# ifndef ERROR_MSG
#  define ERROR_MSG "\033[1;31mERROR\n\033[0m"
# endif

# ifndef EXIT_MSG
#  define EXIT_MSG "\033[1;35mSee you soon, human!\033[0m"
# endif

# ifndef INVALID_USAGE
#  define INVALID_USAGE "invalid usage"
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

// HANDLE ERROS
void						handle_erros(char *msg, int stage, void *ptr);

// UTILS
int							check_quotes(char c, int quotes);
// void	free_matrix(char **matrix);
// int		ft_isspace(char c);
char						**copy_envp(char **envp);
char						*get_env_value(char *path, char **envp);
void						safe_close(int fd1, int fd2, int fd3);

// LEXER
char						**lexer(char *input);

// PARSER
void						insert_token(t_token **head, char *value);
int							is_builtin(char *value);
char						**split_line_arg(char *line);
void						type_token(t_token **head);
int							parser(t_token **head, char *str);

// PARSER UTILS
void						split_line(char *line, char ***split, int *j);
void						check_pipe(char *line, t_token **head);
char						*remove_quotes(char *str);
void						detect_quote_type(char *str, t_token *token);

// PARSER UTILS2
t_token						*create_token(char *value);
void						add_to_list(t_token **tokens, t_token *new);
int							check_redirection(t_token *token);
int							check_command_type(t_token *token);
int							quotes(char c, char *quote_char);

// SYNTAX CHECKER
int							syntax_checker(t_data *data);

// EXPAND
void						expand(t_data *data);
char						*expand_var(char *str, char **envp);

// EXPAND VAR UTILS
void						process_quotes(char c, int *quotes);
char						*extract_var_name(char *str, int *i);
char						*append_char_to_result(char *result, char c);
char						*get_var_value(char *var_name, char **envp);
char						*append_var_value(char *result, char *var_name,
								char **envp);

// EXECUTOR
void						wait_for_children(t_exec *ex, t_data *data);
void						create_child_process(t_data *data, t_exec *ex);
void						executor(t_data *data);
int							execute_builtin(t_data *data, char **cmd);

// EXECUTOR UTILS
t_exec						init_executor(t_data *data);
void						close_all_fds(int **fds, int pipe_count);
void						advance_to_next_cmd(t_exec *ex);

// EXECUTOR CMD
void						child_process(t_data *data, int pipe_index);
char						**create_cmd_array(t_token *tokens);
void						free_pipe_fds(int **fds, int pipe_count);

// EXECUTOR CMD2
void						execute_external(t_data *data, char **cmd);

// SIGNALS
void						handle_sigint(int sig);
void						ignore_signals_in_child(void);
void						setup_signals(void);
void						handle_sigint_heredoc(int sig);
void						setup_signals_heredoc(void);

// FREE
char						*free_ptr(char *ptr);
char						**free_mat(char **mat);
t_token						*free_token(t_token *token);

// HANDLE REDIRS
int							handle_redir_out(char *filename);
int							handle_redir_in(char *filename);
int							handle_redir_append(char *filename);
int							handle_heredoc(char *delimiter);
int							setup_redirections_for_token(t_token *tokens);

// HERE DOC UTILS
int							check_last_delimiter(char *delimiter,
								char **last_delimiter, int *already_processed);
int							handle_heredoc_interrupt(int stdin_copy,
								int *pipefd);
int							process_heredoc_input(char *delimiter, int *pipefd);

// HANDLE REDIRS UTILS
int							prepare_heredoc_pipe(int *pipefd, int *stdin_copy);
int							process_heredoc_line(char *line, char *delimiter,
								int pipefd);
void						cleanup_heredoc(int *pipefd, int stdin_copy);
int							validate_redirection_token(t_token *token);
int							process_redirection(t_token *token);

// CD
void						set_dir(t_data *ms, char *dir, char ***envp);
void						ft_cd(t_data *ms, char **cmd, char ***envp);

// EXPORT
void						ft_export(t_data *ms, char **cmd, char ***envp);

// ENV
void						ft_env(t_data *ms, char **cmd, char ***envp);

// ECHO
void						ft_echo(t_data *ms, char **cmd);

// EXIT
void						ft_exit(t_data *ms, char **cmd);

// PWD
void						ft_pwd(t_data *ms, char **cmd, char **envp);

// UNSET
void						ft_unset(t_data *ms, char **cmd, char ***envp);

#endif