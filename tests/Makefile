#================================FLAGS=========================================#

NAME = minishell_tests
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT_FLAGS = -L$(LIB_PATH) -lft
MAKE_NO_PRINT = $(MAKE) --no-print-directory

#================================PATH==========================================#

SRC_PATH = ../srcs
LIB_PATH = ../libs/libft
INC_PATH = ../includes
TEST_PATH = ./tests

#==============================SOURCES=========================================#

SRCS = $(wildcard $(SRC_PATH)/*.c)
OBJS = $(SRCS:.c=.o)
TEST_SRCS = $(wildcard $(TEST_PATH)/*.c)
TEST_OBJS = $(TEST_SRCS:.c=.o)

#===============================RULES===========================================#

all: $(NAME)

$(NAME): $(OBJS) $(TEST_OBJS)
	@$(MAKE_NO_PRINT) -C $(LIB_PATH)
	@$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS) $(LIBFT_FLAGS) -I$(INC_PATH) -o $(NAME)

clean:
	@echo "Cleaning objects..."
	@rm -f $(OBJS) $(TEST_OBJS)
	@$(MAKE_NO_PRINT) -C $(LIB_PATH) clean

fclean: clean
	@echo "Cleaning executables..."
	@rm -f $(NAME)
	@$(MAKE_NO_PRINT) -C $(LIB_PATH) fclean

re: fclean all

.PHONY: all clean fclean re