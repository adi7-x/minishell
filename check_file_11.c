#include "minishell.h"

int	handle_input_files(t_file *new, t_var_us *var)
{
    if (new->infile == 1)
    {
        if (var->infd != -2)
            close(var->infd);
        var->infd = open(new->file_name, O_RDONLY);
    }
    else if (new->heredoc == 1)
    {
        if (var->infd != -2)
            close(var->infd);
        var->infd = new->fd[0];
    }
    if (var->infd == -1)
        return (1);
    return (0);
}

int	handle_output_files(t_file *new, t_var_us *var)
{
    if (new->outfile == 1)
    {
        if (var->outfd != -2)
            close(var->outfd);
        var->outfd = open(new->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    }
    else if (new->append == 1)
    {
        if (var->outfd != -2)
            close(var->outfd);
        var->outfd = open(new->file_name, O_WRONLY | O_APPEND | O_CREAT, 0664);
    }
    if (var->outfd == -1)
        return (1);
    return (0);
}

int	handle_ambiguous_redirect(t_file *new)
{
    if (new->ambiguous == 1)
    {
        printf("minishell :%s: ambiguous redirect\n", new->file_name);
        return (1);
    }
    return (0);
}

int	check_file(t_data *data, t_var_us *var)
{
    t_file	*new;

    new = data->file;
    var->infd = -2;
    var->outfd = -2;
    while (new)
    {
        if (handle_input_files(new, var) || handle_output_files(new, var))
        {
            perror(new->file_name);
            return (1);
        }
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
            free(file->file_name);
        free(file);
        file = tmp;
    }
}

// void free_envp(t_env *envp)
// {
//     t_env *tmp;

//     while (envp)
//     {
//         tmp = envp->next;
//         if (envp->name)
//             free(envp->name);
//         if (envp->value)
//             free(envp->value);
//         free(envp);
//         envp = tmp;
//     }
// }

void free_data111(t_data *data)
{
    t_data *tmp;

    while (data)
    {
        tmp = data->next;
        ft_free1(data->cmd);
        free_file(data->file);
        free(data);
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
            free(envp->name);
        if (envp->value)
            free(envp->value);
        free(envp);
        envp = tmp;
    }
}

void check_file1(t_data *data, t_var_us *var)
{
    if (check_file(data, var) == 1)
    {
        free_data111(data);
        if (var->envp)
            free_envp(var->envp);
        rl_clear_history();
        exit(1);
    }
}