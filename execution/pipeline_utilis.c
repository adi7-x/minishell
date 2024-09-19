/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utilis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:00:30 by elcid             #+#    #+#             */
/*   Updated: 2024/09/18 20:08:22 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_with_error(char *error_msg)
{
	perror(error_msg);
	gc_free_all();
	exit(1);
}

void	close_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	count_commands_and_create_pipes(t_data *data, int ***pipes)
{
	int		count;
	t_data	*current;
	int		i;

	count = 0;
	current = data;
	while (current)
	{
		count++;
		current = current->next;
	}
	*pipes = gc_malloc((count - 1) * sizeof(int *));
	i = 0;
	while (i < count - 1)
	{
		(*pipes)[i] = gc_malloc(2 * sizeof(int));
		pipe((*pipes)[i]);
		i++;
	}
	return (count);
}
