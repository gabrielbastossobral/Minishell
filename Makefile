#===============================FLAGS==========================================#

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT_FLAGS = -L$(LIB_PATH) -lft -lreadline
MAKE_NO_PRINT = $(MAKE) --no-print-directory

#===============================PATH===========================================#

SRC_PATH = ./srcs
BUILTINS_DIR = $(SRC_PATH)/builtins
CHILD_DIR = $(SRC_PATH)/child
EXECUTOR_DIR = $(SRC_PATH)/executor
EXPAND_DIR = $(SRC_PATH)/expand
PARSER_DIR = $(SRC_PATH)/parser
REDIRECT_DIR = $(SRC_PATH)/redirect
LIB_PATH = ./libs/libft
INC_PATH = ./includes

#==============================SOURCES=========================================#

SRCS =	$(wildcard $(SRC_PATH)/*.c) \
		$(wildcard $(BUILTINS_DIR)/*.c) \
		$(wildcard $(CHILD_DIR)/*.c) \
		$(wildcard $(EXECUTOR_DIR)/*.c) \
		$(wildcard $(EXPAND_DIR)/*.c) \
		$(wildcard $(PARSER_DIR)/*.c) \
		$(wildcard $(REDIRECT_DIR)/*.c)
BUILTIN_SRCS = $(wildcard $(BUILTIN_PATH)/*.c)
SRCS += $(BUILTIN_SRCS)
OBJS = $(SRCS:.c=.o)
HEADERS = $(INC_PATH)/minishell.h

#===============================COLORS=========================================#

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
MAGENTA = \033[0;35m
YELLOW = \033[0;33m

#===============================COMPILING==========================================#

all: libft $(NAME)
	
libft:
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@$(MAKE_NO_PRINT) -C $(LIB_PATH)
	@echo "$(GREEN)Libft compiled!$(RESET)"

$(NAME): $(OBJS) $(HEADERS)
	@echo "$(YELLOW)Compiling minishell...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_FLAGS) -o $(NAME)
	@echo "$(GREEN)Minishell compiled!$(RESET)"

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH)

#===============================CLEANING==========================================#

clean:
	@echo "$(RED)Cleaning minishell objects...$(RESET)"
	@rm -f $(OBJS)
	@echo "$(RED)Minishell objects cleaned!$(RESET)"
	@echo "$(RED)Cleaning libft objects...$(RESET)"
	@$(MAKE_NO_PRINT) -C $(LIB_PATH) clean
	@echo "$(RED)Libft objects cleaned!$(RESET)"

fclean: clean
	@echo "$(RED)Cleaning minishell...$(RESET)"
	@rm -f $(NAME)
	@echo "$(RED)Minishell cleaned!$(RESET)"
	@echo "$(RED)Cleaning libft...$(RESET)"
	@$(MAKE_NO_PRINT) -C $(LIB_PATH) fclean
	@echo "$(RED)Libft cleaned!$(RESET)"

re: fclean all

#===============================LEAK============================================#

leak: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

#===============================GDB=============================================#

gdb: all
	@echo "$(MAGENTA)Running gdb...$(RESET)"
	gdb --tui ./$(NAME)

#===============================NORM===========================================#

norm:
	norminette $(SRC_PATH) $(INC_PATH)

#===============================SET===========================================#

set:
	@git submodule update --init --recursive

#===============================PHONY==========================================#

.PHONY: all clean fclean re leak gdb norm libft