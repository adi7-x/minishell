#include "minishell.h"

int	handle_ambiguous_redirect(t_file *new)
{
    if (new->ambiguous == 1)
    {
        printf("minishell :%s: ambiguous redirect\n", new->file_name);
        return (1);
    }
    return (0);
}

int	check_file(t_data *data)
{
    t_file	*new;

    new = data->file;
    while (new)
    {

        if (handle_ambiguous_redirect(new))
        {
            return (1);
        }
        new = new->next;
    }
    return (0);
}

void free_file(t_file *file)
{
    t_file *tmp;

    while (file)
    {
        tmp = file->next;
        if (file->file_name)
            gc_remove_ptr(file->file_name);
        gc_remove_ptr(file);
        file = tmp;
    }
}

void free_data111(t_data *data)
{
    t_data *tmp;

    while (data)
    {
        tmp = data->next;
        ft_free1(data->cmd);
        free_file(data->file);
        gc_remove_ptr(data);
        data = tmp;
    }
}


void free_envp(t_env *envp)
{
    t_env *tmp;

    while (envp)
    {
        tmp = envp->next;
        if (envp->name)
            gc_remove_ptr(envp->name);
        if (envp->value)
            gc_remove_ptr(envp->value);
        gc_remove_ptr(envp);
        envp = tmp;
    }
}



int check_file1(t_data *data)
{
    if (check_file(data) == 1)
    {
        return 1;
    }
    return 0;
}
