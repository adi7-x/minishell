/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 22:41:21 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 17:01:50 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# define MAX_PATH 1024

# define TOKEN_WORD 1
# define TOKEN_PIPE 2
# define TOKEN_INREDIR 3
# define TOKEN_OUTREDIR 4
# define TOKEN_HEREDOC 5
# define TOKEN_REDIR_APPEND 6
# define TOKEN_SEMICOLON 7

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

typedef struct s_shell
{
	char				**env;
	char				cwd[MAX_PATH];
	int					ignore_path;
}						t_shell;

extern t_global			g_global;

char					*expending_herd(char *str, char **env);
void					*gc_malloc(size_t size);
void					gc_free_all(void);
void					gc_remove_ptr(void *ptr);
char					*gc_strdup(char *s);
char					*gc_strndup(char *s, size_t n);

void					free_env(t_env *env);
void					free_files(t_file *file);
void					free_parsed_data(t_data *data);
void					free_lexer(t_lexer *lexer);

int						handle_pipe(t_lexer **lexer, char *input);
void					ft_lstadd_back(t_lexer **lst, t_lexer *new);
void					lexer_analysis(char *input, t_lexer **lexer);
void					free_lexer(t_lexer *lexer);
void					free_parsed_data(t_data *data);
int						validate_quotes(char *str);

char					*ft_itoa(int nb);

char					*get_environment_variable(char **env, char *str);
char					*append_char_to_string(char *str, char c);
void					append_char_to_new_string(char *str, t_var *var);
char					*join_strings(char *s1, char *ss2);

char					**concatenate_strings(char **str, char **str1);
void					expand_environment_variable(t_var *var, char **envp,
							int flag);
void					handle_expansion_condition(t_var *var, char *str,
							char **envp, int flg);
char					**expand_word_with_variables(char *str, char **envp,
							int flg);
char					**add_command_string(char **str, t_lexer *lexer,
							char **envp);
void					handle_file_redirection(t_lexer *lexer, int type,
							t_file **file, char **envp);
t_data					*parse_lexer_to_data(t_lexer *lexer, char **envp);
t_data					*convert_input_to_data(char *input, t_shell *shell);
int						count_string_array(char **str);
void					handle_ambiguous_redirection(char **namfile,
							t_file **file, t_file *newfile, char *s);

int						copy_string_array(char **cmd, char **str);
char					*ft_substr(char *s, int start, int len);
void					check_for_variable_expansion(char *str, t_var *var);
int						handle_exit_status(t_var *var);
int						calculate_length(long nb);
char					*ft_strsrch(char *str, char c);
char					*remove_qout(char *str);
void					add_parsed_data(t_data **data, t_file **file,
							char ***cmd);
int						ft_herdoc(t_data *data, char **env);
void					free_herdoc_and_exit(int exit_s, char *str);
int						handle_ambiguous_redirect(t_file *new);

int						is_token_character(char c, int *single_quote_state,
							int *double_quote_state);

void					ft_lstadd_back(t_lexer **lst, t_lexer *new);

void					append_token_to_lexer(t_lexer **lexer, char *token_data,
							int token_type);
int						handle_output_redirection(t_lexer **lexer, char *input);
int						handle_input_redirection(t_lexer **lexer, char *len);
int						handle_pipe(t_lexer **lexer, char *len);
int						handle_word(t_lexer **lexer, char *len);
void					lexer_analysis(char *input, t_lexer **lexer);

void					sigint_handler(int sig);
void					setup_signals(void);
void					handle_heredoc_signal(int sig);

void					free_data(t_data *data);
void					cleanup_shell(void);

int						builtin_cd(t_shell *shell, t_data *data);
int						builtin_pwd(t_shell *shell);
int						builtin_unset(t_shell *shell, t_data *data);
int						builtin_env(t_shell *shell);
int						builtin_exit(t_data *data);
int						builtin_echo(t_data *data);

int						ft_unsetenv(t_shell *shell, char *name);
int						execute_pipeline(t_shell *shell, t_data *data);
int						execute_pipeline(t_shell *shell, t_data *data);
void					create_pipes(int ***pipes, int cmd_count);
int						count_commands(t_data *data);
void					close_pipes(int **pipes, int cmd_count);
void					setup_child_pipes(int **pipes, int i, int cmd_count);

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
void					handle_command(t_shell *shell, t_data *data);
void					initialize_shell(t_shell *shell, char **envp);
int						execute_builtin(t_shell *shell, t_data *data);
int						is_builtin(char *cmd);
char					*ft_strtok(char *str, char sepa);
char					*strjoin(char *s1, char *s2, char *delimiter);

void					exit_with_error(char *error_msg);
void					handle_execve_error(char *path);
void					free_data(t_data *data);

int						handle_semicolon(t_lexer **lexer, char *input);
int						process_syntax_check(t_lexer **lexer);
int						ft_strlen(char *str);
int						ft_strcmp(const char *s1, const char *s2);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
void					*ft_memset(void *b, int c, size_t len);
void					*ft_memcpy(void *dst, const void *src, size_t n);
char					*ft_strcpy(char *dest, const char *src);
char					*ft_strchr(const char *s, int c);
int						ft_isalpha(int c);
int						ft_isalnum(int c);
int						is_whitespace_char(char c);
void					*ft_calloc(size_t count, size_t size);
char					*ft_strncpy(char *dest, char *src, int size);
char					**ft_split(char *str);
void					free_string_array(char **str);
void					print_error_not_such_file(char *file_name);
void					init_var(t_var *var);
void					handle_quotes(char c, t_var *var);
void					handle_dollar_sign(t_var *var, char *str, char **envp,
							int flg);
char					*append_ctos(char *str, char c);

#endif
