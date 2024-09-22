/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utilis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 15:46:15 by elcid             #+#    #+#             */
/*   Updated: 2024/09/22 16:14:08 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	setup_child_pipes(int **pipes, int i, int cmd_count)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_pipes(pipes, cmd_count);
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

int	count_commands(t_data *data)
{
	int		count;
	t_data	*current;

	count = 0;
	current = data;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	create_pipes(int ***pipes, int cmd_count)
{
	int	i;

	*pipes = gc_malloc((cmd_count - 1) * sizeof(int *));
	i = 0;
	while (i < cmd_count - 1)
	{
		(*pipes)[i] = gc_malloc(2 * sizeof(int));
		pipe((*pipes)[i]);
		i++;
	}
}
