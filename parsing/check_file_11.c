/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file_11.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:09:10 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/20 16:52:26 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_file(t_file *file)
{
	t_file	*tmp;

	while (file)
	{
		tmp = file->next;
		if (file->file_name)
			gc_remove_ptr(file->file_name);
		gc_remove_ptr(file);
		file = tmp;
	}
}

void	free_data111(t_data *data)
{
	t_data	*tmp;

	while (data)
	{
		tmp = data->next;
		free_file(data->file);
		gc_remove_ptr(data);
		data = tmp;
	}
}

void	free_envp(t_env *envp)
{
	t_env	*tmp;

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

int	check_file(t_data *data)
{
	t_data	*tmp;

	tmp = data;
	while (tmp)
	{
		if (tmp->cmd == NULL)
		{
			free_data111(data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	check_file1(t_data *data)
{
	if (check_file(data) == 1)
	{
		return (1);
	}
	return (0);
}
