CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

NAME = minishell

      
SRCS = minishell.c \
      functions_utils1/clean_up.c \
      functions_utils1/functions_utils.c \
      parsing/string_utils_2.c \
      parsing/string_utils.c \
      parsing/variable_expansion_utils.c \
      parsing/data_parsing_utils.c  \
      parsing/env_and_array_utils.c \
      parsing/redirection_handling_utils.c  \
      parsing/validate_quotes.c \
      parsing/heredoc_expansion_utils.c \
      parsing/lexer_analysis.c \
      parsing/syntax_error.c \
      parsing/lexer_utils.c \
      parsing/variable_expansion.c \
      parsing/variable_expansion_utils2.c \
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
