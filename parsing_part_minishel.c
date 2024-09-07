#include "minishell.h"

// Existing function prototypes...

void	add_env_var(t_env **env, char *env_var)
{
	t_env	*new_node;
	t_env	*current;
	char	*name;
	char	*value;
	char	*delimiter;

	delimiter = strchr(env_var, '=');
	if (!delimiter)
		return ;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	name = strndup(env_var, delimiter - env_var);
	value = strdup(delimiter + 1);
	if (!name || !value)
	{
		perror("strdup");
		free(new_node);
		free(name);
		free(value);
		exit(EXIT_FAILURE);
	}
	new_node->name = name;
	new_node->value = value;
	new_node->next = NULL;
	if (*env == NULL)
	{
		*env = new_node;
	}
	else
	{
		current = *env;
		while (current->next)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

char	**add_command_argument(char **cmd, char *new_arg)
{
	int		i;
	char	**new_cmd;

	// Count the current number of arguments
	for (i = 0; cmd && cmd[i]; i++)
		;
	// Allocate space for one more argument and the NULL terminator
	new_cmd = malloc(sizeof(char *) * (i + 2));
	if (!new_cmd)
		return (NULL);
	// Copy the existing arguments
	for (int j = 0; j < i; j++)
		new_cmd[j] = cmd[j];
	// Add the new argument
	new_cmd[i] = strdup(new_arg);
	new_cmd[i + 1] = NULL;
	// Free the old cmd array (not the strings inside)
	free(cmd);
	return (new_cmd);
}

t_file	*add_redirection(t_file *file_list, t_lexer *current_token)
{
	t_file	*new_file;
	t_file	*tmp;

	new_file = malloc(sizeof(t_file));
	if (!new_file)
		return (NULL);
	new_file->next = NULL;
	new_file->file_name = NULL;
	new_file->infile = (current_token->token.type == TOKEN_INREDIR);
	new_file->outfile = (current_token->token.type == TOKEN_OUTREDIR);
	new_file->append = (current_token->token.type == TOKEN_REDIR_APPEND);
	new_file->heredoc = (current_token->token.type == TOKEN_HEREDOC);
	// Move to the next token to get the filename
	if (current_token->next && current_token->next->token.type == TOKEN_WORD)
	{
		new_file->file_name = strdup(current_token->next->data);
	}
	else
	{
		free(new_file);
		return (file_list); // Return the original list if there's no filename
	}
	// Add the new redirection to the file list
	if (!file_list)
		return (new_file);
	else
	{
		tmp = file_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_file;
	}
	return (file_list);
}

t_data	*ft_parser(t_lexer *lexer_output)
{
	t_data	*data_head;
	t_data	*current_data;
	t_lexer	*current_token;
	t_data	*new_data;

	data_head = NULL;
	current_data = NULL;
	current_token = lexer_output;
	while (current_token)
	{
		new_data = malloc(sizeof(t_data));
		if (!new_data)
			return (NULL);
		new_data->cmd = NULL;
		new_data->file = NULL;
		new_data->next = NULL;
		while (current_token && current_token->token.type != TOKEN_PIPE)
		{
			if (current_token->token.type == TOKEN_WORD)
			{
				new_data->cmd = add_command_argument(new_data->cmd,
						current_token->data);
			}
			else if (current_token->token.type == TOKEN_INREDIR
				|| current_token->token.type == TOKEN_OUTREDIR
				|| current_token->token.type == TOKEN_REDIR_APPEND
				|| current_token->token.type == TOKEN_HEREDOC)
			{
				new_data->file = add_redirection(new_data->file, current_token);
				if (current_token->next
					&& current_token->next->token.type == TOKEN_WORD)
					current_token = current_token->next; // Skip the filename
			}
			current_token = current_token->next;
		}
		if (!data_head)
			data_head = new_data;
		else
			current_data->next = new_data;
		current_data = new_data;
		if (current_token && current_token->token.type == TOKEN_PIPE)
			current_token = current_token->next;
	}
	return (data_head);
}

t_data	*parse_input(char *input, t_shell *shell)
{
	t_lexer	*lexer_output;
	t_data	*data;

	data = NULL;
	lexer_output = NULL;
	lexer_analysis(input, &lexer_output);
	if (!lexer_output)
		return (NULL);
	if (!expander(lexer_output, shell->env))
	{
		free_lexer(lexer_output);
		return (NULL);
	}
	data = ft_parser(lexer_output); // Parse the lexer output into commands
	if (!data)
	{
		free_lexer(lexer_output);
		return (NULL);
	}
	free_lexer(lexer_output); // Clean up lexer tokens
	return (data);
}

// t_data	*parse_input(char *input, t_env *env)
// {
// 	t_lexer	*lexer_output;
// 	t_data	*data;

// 	// t_lexer	*current;
// 	data = NULL;
// 	lexer_output = NULL;
// 	lexer_analysis(input, &lexer_output);
// 	if (!lexer_output)
// 		return (NULL);
// 	if (!expander(lexer_output, env))
// 	{
// 		free_lexer(lexer_output);
// 		return (NULL);
// 	}
// 	data = malloc(sizeof(t_data));
// 	if (!data)
// 	{
// 		free_lexer(lexer_output);
// 		return (NULL);
// 	}
// 	free_lexer(lexer_output);
// 	return (data);
// }

// this is work but before merge the execuction part
// void	parse_and_execute(char *input, t_env *env)
// {
// 	t_data	*data;

// 	data = parse_input(input, env);
// 	if (!data)
// 	{
// 		printf("syntax error\n");
// 		return ;
// 	}

// }

// int	main(int argc, char **argv, char **envp)
// {
// 	char	*input;
// 	t_env	*env;

// 	(void)argc;
// 	(void)argv;
// 	env = init_env(envp);
// 	// TODO: Implement setup_signals();
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input)
// 		{
// 			printf("exit\n");
// 			break ;
// 		}

// 		if (*input)
// 		{
// 			add_history(input);
// 			parse_and_execute(input, env);
// 		}
// 		free(input);
// 	}
// 	free_env(env);
// 	return (g_exit_status);
// }
