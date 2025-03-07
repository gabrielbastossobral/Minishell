#================================FLAGS=========================================#

NAME = test
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT_FLAGS = -lreadline -L$(LIB_PATH) -lft
MAKE_NO_PRINT = $(MAKE) --no-print-directory

#================================PATH==========================================#

SRC_PATH = ../srcs
HEADER = ../includes
LIB_PATH = ../libs/libft
TEST_PATH = ./tests

#==============================SOURCES=========================================#

SRCS = $(SRC_PATH)/utils.c \
       $(SRC_PATH)/parser.c \
       $(SRC_PATH)/parser_utils.c \
       $(SRC_PATH)/lexer.c \
	   $(SRC_PATH)/handle_erros.c\
	   $(SRC_PATH)/syntax_checker.c\
	   $(SRC_PATH)/expand.c\
	   $(SRC_PATH)/expand_var_utils.c\
	   $(SRC_PATH)/free.c\
	   $(SRC_PATH)/builtins/cd.c\
	   $(SRC_PATH)/builtins/export.c\
	   $(SRC_PATH)/builtins/env.c\
	   $(SRC_PATH)/builtins/echo.c\
	   $(SRC_PATH)/builtins/exit.c\
	   $(SRC_PATH)/builtins/pwd.c

OBJS = $(SRCS:.c=.o)
TEST_SRCS = $(wildcard *.c)
TEST_OBJS = $(TEST_SRCS:.c=.o)

#===============================RULES===========================================#

run: all
	@valgrind ./$(NAME)

all: $(NAME)

$(NAME): $(OBJS) $(TEST_OBJS)
	$(MAKE_NO_PRINT) -C $(LIB_PATH)
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) -I$(HEADER) $(LIBFT_FLAGS) -o $(NAME)

clean:
	@echo "Cleaning objects..."
	@$(MAKE_NO_PRINT) -C $(LIB_PATH) fclean
	@rm -f $(OBJS) $(TEST_OBJS)

fclean: clean
	@echo "Cleaning executables..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re