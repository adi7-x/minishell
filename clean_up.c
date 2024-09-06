#include "minishell.h"

void	free_lexer(t_lexer *lexer)
{
	t_lexer	*current;
	t_lexer	*next;

	current = lexer;
	while (current)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
}

void	free_parsed_data(t_data *data)
{
	// Implement this function based on your t_data structure
	// For now, we'll just free the data pointer itself
	free(data);
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
}
