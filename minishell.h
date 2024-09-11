#ifndef MINISHELL_H
# define MINISHELL_H

#include "minishell.h"
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#define MAX_PATH 1024

typedef struct s_shell
{
    char **env;
    int ignore_path;
    char cwd[MAX_PATH]; 
} t_shell;

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_INREDIR,
    TOKEN_OUTREDIR,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC
}                       t_token_type;

typedef struct s_token
{
    char                *value;
    t_token_type        type;
    struct s_token      *next;
}                       t_token;

typedef struct s_env
{
    char                *name;
    char                *var;
    char                *value;
    int                 exit_status;
    struct s_env        *next;
}                       t_env;

typedef struct s_file
{
    char                *file_name;
    int                 infile;
    int                 outfile;
    int                 heredoc;
    int                 append;
    int                 ambiguous;
    int                 fd[2];
    struct s_file       *next;
}                       t_file;

typedef struct t_data
{
    char                **cmd;
    t_file              *file;
    struct t_data       *next;
}                       t_data;

typedef struct s_command
{
    char                **args;
    t_file              *redirects;
    struct s_command    *next;
}                       t_command;

typedef struct t_lexer
{
    char                *data;
    t_token             token;
    struct t_lexer      *next;
    struct t_lexer      *prev;
}                       t_lexer;

typedef struct t_var
{
    int                 sinqot;
    int                 dobqot;
    int                 i;
    int                 j;
    char                *var;
    char                *newstr;
}                       t_var;

typedef struct s_gc_node
{
    void *ptr;
    int is_freed;
    struct s_gc_node *next;
} t_gc_node;

typedef struct s_global
{
    t_gc_node *gc_list;
    int exit_number;
    int signal_received;
    int is_main_shell;
} t_global;

extern t_global g_global;

// garbage collector functions

void    *gc_malloc(size_t size);
void    gc_free_all(void);
void    gc_remove_ptr(void *ptr);
char    *gc_strdup(const char *s);

int ft_herdoc(t_data *data, char **env);

void                    free_env(t_env *env);
void                    free_files(t_file *file);
void                    free_commands(t_command *commands);
void                    free_parsed_data(t_data *data);
void                    free_lexer(t_lexer *lexer);

int                     get_quoted_word(t_lexer **lexer, char *input);
int                     handle_redirection(t_lexer **lexer, char *input);
int                     handle_pipe(t_lexer **lexer, char *input);
int                     extract_word(t_lexer **lexer, char *input);
void                    ft_lstadd_back(t_lexer **lst, t_lexer *new);
void                    add_token(t_lexer **lexer, char *value,
                            enum e_token_type type);
int                     get_next_quote(char *input);
void                    lexer_analysis(char *input, t_lexer **lexer);
int                     check_syntax_errors(t_lexer *lexer);
void                    free_lexer(t_lexer *lexer);
void                    free_parsed_data(t_data *data);
int                     check_quotes(char *str);
int                     expander(t_lexer *lexer_output, char **env); // Updated to match the definition in expande.c
char                    *apend_char_str(char *str, char c);

char                    *ft_getenv(char **env, char *str); // Updated to match the definition in expande.c
char                    *expend_it(char *var, char *newstr, char **envp); // Updated to match the definition in expande.c
char                    *add_to_str(char *str, t_var var, int *i);
char                    *ft_expending_word(char *str, char **envp); // Updated to match the definition in expande.c
char                    *ft_itoa(int nb);
t_data                  *parse_input(char *input, t_shell *shell);

char *int_to_str(int nbr);

#endif

