#ifndef MINISHELL_H
# define MINISHELL_H

# include <ctype.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_INREDIR,
	TOKEN_OUTREDIR,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}						t_token_type;
;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
}						t_token;

typedef struct s_env
{
	char				*name;
	char				*var;
	char				*value;
	int					exit_status;
	struct s_env		*next;
}						t_env;

typedef struct s_file
{
	char				*name;
	int					infile;
	int					outfile;
	int					heredoc;
	int					append;
	int					ambiguous;
	int					fd[2];
	struct s_file		*next;
}						t_file;

typedef struct t_data
{
	char				**cmd;
	char				*oldpwd;
	char				*newpwd;
	t_file				*file;
	struct t_data		*next;
}						t_data;

typedef struct s_command
{
	char				**args;
	t_file				*redirects;
	struct s_command	*next;
}						t_command;

typedef struct t_lexer
{
	char				*data;
	t_token				token;
	struct t_lexer		*next;
	struct t_lexer		*prev;
}						t_lexer;

typedef struct t_var
{
	int					sinqot;
	int					dobqot;
	int					i;
	int					j;
	char				*var;
	char				*newstr;
}						t_var;

extern int				g_exit_status;

void					free_env(t_env *env);
void					free_files(t_file *file);
void					free_commands(t_command *commands);
void					free_parsed_data(t_data *data);
void					free_lexer(t_lexer *lexer);

int						get_quoted_word(t_lexer **lexer, char *input);
int						handle_redirection(t_lexer **lexer, char *input);
int						handle_pipe(t_lexer **lexer, char *input);
int						extract_word(t_lexer **lexer, char *input);
void					ft_lstadd_back(t_lexer **lst, t_lexer *new);
void					add_token(t_lexer **lexer, char *value,
							enum e_token_type type);
void					free_env_list(t_env **env_p);
int						get_next_quote(char *input);
void					lexer_analysis(char *input, t_lexer **lexer);
int						check_syntax_errors(t_lexer *lexer);
void					free_lexer(t_lexer *lexer);
void					free_parsed_data(t_data *data);
int						check_quotes(char *str);
int						expander(t_lexer *lexer_output, t_env *env);
char					*apend_char_str(char *str, char c);

char					*ft_getenv(t_env *env, char *str);
char					*expend_it(char *var, char *newstr, t_env *envp);
char					*add_to_str(char *str, t_var var, int *i);
char					*ft_expending_word(char *str, t_env *envp);
char					*ft_itoa(int nb);

#endif
