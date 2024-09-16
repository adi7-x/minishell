CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

NAME = minishell

SRCS = lexer_analysis.c  syntax_error.c clean_up.c \
		check_quotes.c minishell.c garbage_collector.c ft_parsing_end_expand.c ft_herdoc.c ft_expande_her.c check_file_11.c \
	

OBJS = $(SRCS:.c=.o)

HEADER = minishell.h

LIBS = -lreadline

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./$(NAME)
