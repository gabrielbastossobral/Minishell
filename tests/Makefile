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

SRCS = $(wildcard $(SRC_PATH)/utils.c \
							parser.c \
							parser_utils.c \
							lexer.c \)
OBJS = $(SRCS:.c=.o)
TEST_SRCS = $(wildcard *.c)
TEST_OBJS = $(TEST_SRCS:.c=.o)

#===============================RULES===========================================#

all: $(NAME)

$(NAME): $(OBJS) $(TEST_OBJS)
	$(MAKE_NO_PRINT) -C $(LIB_PATH)
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) -I$(HEADER) $(LIBFT_FLAGS) -o $(NAME)

clean:
	@echo "Cleaning objects..."
	@rm -f $(OBJS) $(TEST_OBJS) $(LIB_PATH)/*.o

fclean: clean
	@echo "Cleaning executables..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re