CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

NAME = minishell

SRCS = check_file_11.c \
      check_quotes.c \
      clean_up.c \
      ft_expande_her.c \
      ft_parsing_end_expand.c \
      lexer_analysis.c \
      minishell.c \
      syntax_error.c \
      lexer_utils.c \
      functions_utils.c \
      execution/ft_herdoc.c \
      execution/garbage_collector.c \
      execution/pipeline.c \
      execution/pipeline_utilis.c \
      execution/setenv.c \
      execution/signal_handlers.c \
      execution/simple_command.c \
      execution/handle_command.c \
      execution/initialize_shell.c \
      execution/sort_export.c \
      execution/strcmp.c \
      execution/handle_redirections.c \
      execution/errors.c \
      execution/helpers1.c \
	execution/cleanup_exe.c \
      execution/execute_builtin.c \
      execution/builtin_export.c \
      execution/builtin_cd_pwd.c \
      execution/builtin_unset.c \
      execution/builtin_env.c \
      execution/builtin_exit.c \
      execution/builtin_echo.c \


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
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp -s ./$(NAME)