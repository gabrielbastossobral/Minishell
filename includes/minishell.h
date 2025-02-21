# ifndef MINISHELL_H
# define MINISHELL_H

#include "readline/readline.h"
#include "../libs/libft/includes/not_in_standard_includes/ft_printf_bonus.h"

// COLORS

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define RESET "\033[0m"

// HANDLE ERROS

void handle_erros(char *msg, int stage);

# endif