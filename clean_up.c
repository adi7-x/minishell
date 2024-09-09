#include "minishell.h"

// void	free_lexer(t_lexer *lexer)
// {
// 	t_lexer	*current;
// 	t_lexer	*next;

// 	current = lexer;
// 	while (current)
// 	{
// 		next = current->next;
// 		gc_remove_ptr(current->data);
// 		gc_remove_ptr(current);
// 		current = next;
// 	}
// }
void free_lexer(t_lexer *lexer)
{
    t_lexer *temp;

    while (lexer)
    {
        temp = lexer;
        lexer = lexer->next;
        free(temp->data); 
        free(temp);       
    }
}
void free_files(t_file *file)
{
    t_file *tmp;

    while (file)
    {
        tmp = file;
        file = file->next;
        gc_remove_ptr(tmp->file_name);
        gc_remove_ptr(tmp);
    }
}
void free_parsed_data(t_data *data)
{
    t_data *tmp;

    while (data)
    {
        tmp = data;
        data = data->next;
        if (tmp->cmd)
            gc_remove_ptr(tmp->cmd);
        if (tmp->file)
            free_files(tmp->file);
        gc_remove_ptr(tmp);
    }
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		gc_remove_ptr(current->name);
		gc_remove_ptr(current->value);
		gc_remove_ptr(current);
		current = next;
	}
}
