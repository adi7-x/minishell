CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

NAME = minishell

SRCS = lexer_analysis.c parsing_part_minishel.c  syntax_error.c clean_up.c \
		check_quotes.c expande.c minishell.c garbage_collector.c ft_herdoc.c \

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

.PHONY: all clean fclean re
