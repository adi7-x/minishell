#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# define MAX_PATH 1024

// typedef struct s_shell
// {
// 	char				**env;
// 	int					ignore_path;
// 	char				cwd[MAX_PATH];
// }						t_shell;

# define TOKEN_WORD 1
# define TOKEN_PIPE 2
# define TOKEN_INREDIR 3
# define TOKEN_OUTREDIR 4
# define TOKEN_HEREDOC 5
# define TOKEN_REDIR_APPEND 6

typedef struct s_env
{
	char				*name;
	char				*var;
	char				*value;
	int					exit_status;
	struct s_env		*next;
}						t_env;

typedef struct s_token
{
	char				*value;
	int					type;
	struct s_token		*next;
}						t_token;

typedef struct s_file
{
	char				*file_name;
	int					infile;
	int					outfile;
	int					heredoc;
	int					append;
	int					ambiguous;
	int					fd[2];
	int					expand;
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
	int					type;
	struct t_lexer		*next;
	struct t_lexer		*prev;
}						t_lexer;

typedef struct t_var
{
	char				**cmd;
	t_file				*file;
	t_lexer				*lexer;
	int					single_quote;
	int					double_quote;
	int					i;
	int					j;
	char				*var;
	char				**new_str;
	int					count;
	char				*str;
	int					flag;
}						t_var;

typedef struct s_gc_node
{
	void				*ptr;
	int					is_freed;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_global
{
	t_gc_node			*gc_list;
	int					exit_number;
	int					signal_received;
	int					is_main_shell;
}						t_global;

typedef struct t_var_us
{
	int					cont_cmd;
	char				**path;
	char				*pth;
	int					fd;
	int					infd;
	int					outfd;
	int					**pipe;
	int					*id;
	int					i;
	int					n;
	t_env				*envp;
}						t_var_us;

typedef struct s_shell
{
	char				**env;
	char				cwd[MAX_PATH];
	int					ignore_path;
	t_var_us var_us; // Add this line
}						t_shell;

extern t_global			g_global;

char					*expending_herd(char *str, char **env);
int						ft_token(char c, int *singl, int *doubl);
int						is_whitespace(char c);
char					*ft_strncpy(char *dest, char *src, int size);
int						ft_strlen(char *str);
void					*ft_calloc(size_t count, size_t size);
void					*gc_malloc(size_t size);
void					gc_free_all(void);
void					gc_remove_ptr(void *ptr);
char					*gc_strdup(const char *s);
char					*gc_strndup(const char *s, size_t n);

void					free_env(t_env *env);
void					free_files(t_file *file);
void					free_commands(t_command *commands);
void					free_parsed_data(t_data *data);
void					free_lexer(t_lexer *lexer);

int						get_quoted_word(t_lexer **lexer, char *input);
int						handle_redirection(t_lexer **lexer, char *input);
int						handle_pipe(t_lexer **lexer, char *input);
int						ft_token(char c, int *singl, int *doubl);
int						extract_word(t_lexer **lexer, char *input);
void					ft_lstadd_back(t_lexer **lst, t_lexer *new);
void					add_token(t_lexer **lexer, char *value, int type);
int						get_next_quote(char *input);
void					lexer_analysis(char *input, t_lexer **lexer);
void					free_lexer(t_lexer *lexer);
void					free_parsed_data(t_data *data);
int						check_quotes(char *str);
int						expander(t_lexer *lexer_output, char **env);

char					*apend_char_str(char *str, char c);

char					*ft_itoa(int nb);
t_data					*parse_input(char *input, t_shell *shell);

int						c_s_e(t_lexer *lexer);

int						check_syntax_errors(t_lexer **lexer);
char					*ft_getenv(char **env, char *str);
char					*apend_char_str(char *str, char c);
void					add_to_str(char *str, t_var *var);
char					*ft_strjoinn(char *s1, char *ss2);

char					**ft_catstr(char **str, char **str1);
void					expand_var(t_var *var, char **envp, int flag);
void					other_condition(t_var *var, char *str, char **envp,
							int flg);
char					**ft_expending_word(char *str, char **envp, int flg);
char					**ft_addstring(char **str, t_lexer *lexer, char **envp);
void					append_to_file(t_lexer *lexer, int type, t_file **file,
							char **envp);
t_data					*ft_parsing(t_lexer *lexer, char **envp);
t_data					*process_input(char *input, t_shell *shell);
int						count_str(char **str);
void					ft_free1(char **str);
void					ft_ambiguous(char **namfile, t_file **file,
							t_file *newfile, char *s);

int						ft_copy(char **cmd, char **str);
int						cont_w(const char *str, char c);
char					**ft_split(char *s, char c);
char					*ft_substr(char *s, int start, int len);
void					check_word_expand(char *str, t_var *var);
char					*ft_itoa(int nb);
int						ft_exit_status(t_var *var);
int						ft_len(long nb);
char					*ft_strsrch(char *str, char c);
char					*remove_qout(char *str);
void					append_to_data(t_data **data, t_file **file,
							char ***cmd);
t_env					*convert_env_to_list(char **env);
void					free_env_list(t_env *env_list);
int						ft_herdoc(t_data *data, char **env);

int						check_file1(t_data *data);
// int	handle_input_files(t_file *new, t_var_us *var);
// int	handle_output_files(t_file *new, t_var_us *var);
int						handle_ambiguous_redirect(t_file *new);
int						check_file(t_data *data);
// void	check_file1(t_data *data, t_var_us *var);

void					free_var(t_var_us *var);
void					free_file(t_file *file);
void					free_data111(t_data *data);
void					free_envp(t_env *envp);

int						is_token_character(char c, int *single_quote_state,
							int *double_quote_state);
char					*ft_strncpy(char *dest, char *src, int size);
int						is_whitespace_char(char c);

void					*ft_calloc(size_t count, size_t size);
void					ft_lstadd_back(t_lexer **lst, t_lexer *new);
int						ft_strlen(char *str);
void					append_token_to_lexer(t_lexer **lexer, char *token_data,
							int token_type);
int						handle_output_redirection(t_lexer **lexer, char *input);
int						handle_input_redirection(t_lexer **lexer, char *len);
int						handle_pipe(t_lexer **lexer, char *len);
int						handle_word(t_lexer **lexer, char *len);
void					lexer_analysis(char *input, t_lexer **lexer);

// execute functions
void					sigint_handler(int sig);
void					setup_signals(void);
void					handle_heredoc_signal(int sig);

void					free_data(t_data *data);
void					cleanup(void);

int	builtin_cd(t_shell *shell, t_data *data);
int	builtin_pwd(t_shell *shell);
int	builtin_unset(t_shell *shell, t_data *data);
int	builtin_env(t_shell *shell);

int	ft_unsetenv(t_shell *shell, char *name);
int						execute_pipeline(t_shell *shell, t_data *data);
char					*find_command(t_shell *shell, char *cmd);
int						handle_redirections(t_file *file);
int						is_builtin(char *cmd);
int						execute_builtin(t_shell *shell, t_data *data);
int						builtin_export(t_shell *shell, t_data *data);
void					sort_export(t_shell *shell);
int						ft_setenv(t_shell *shell, char *key, char *value,
							int overwrite);
int						is_inenv(char **env, char *name);
int						custom_strcmp(char *a, char *b);
char					*strjoin(char *s1, char *s2, char *delimiter);
int						execute_command(t_shell *shell, t_data *data);
char					*find_command(t_shell *shell, char *cmd);
char					*ft_strtok(char *str, char sepa);
void					exit_with_error(char *error_msg);
void					free_data(t_data *data);

void    ft_handle_execve_error(char *path);
#endif
