/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_exe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 11:13:22 by elcid             #+#    #+#             */
/*   Updated: 2024/09/18 19:50:27 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup(void)
{
	gc_free_all();
	rl_clear_history();
}

void	free_files(t_file *file)
{
	t_file	*next_file;

	while (file)
	{
		next_file = file->next;
		gc_remove_ptr(file->file_name);
		gc_remove_ptr(file);
		file = next_file;
	}
}

void	free_data(t_data *data)
{
	t_data	*next;
	int		i;

	while (data)
	{
		next = data->next;
		if (data->cmd)
		{
			i = 0;
			while (data->cmd[i])
			{
				gc_remove_ptr(data->cmd[i]);
				i++;
			}
			gc_remove_ptr(data->cmd);
		}
		if (data->file)
			free_files(data->file);
		gc_remove_ptr(data);
		data = next;
	}
}
