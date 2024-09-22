/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:18:56 by elcid             #+#    #+#             */
/*   Updated: 2024/09/22 19:48:04 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_heredoc(t_file *file)
{
	dup2(file->fd[0], STDIN_FILENO);
	close(file->fd[0]);
	close(file->fd[1]);
	return (0);
}

int	handle_infile(t_file *file)
{
	int	fd;

	fd = open(file->file_name, O_RDONLY);
	if (fd == -1)
		return (-1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	handle_outfile(t_file *file)
{
	int	fd;

	fd = open(file->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		return (-1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_append(t_file *file)
{
	int	fd;

	fd = open(file->file_name, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd == -1)
		return (-1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(t_file **file)
{
	t_file	*current;
	int		result;

	current = *file;
	while (current)
	{
		if (current->heredoc)
			result = handle_heredoc(current);
		else if (current->outfile)
			result = handle_outfile(current);
		else if (current->infile)
			result = handle_infile(current);
		else if (current->append)
			result = handle_append(current);
		else if (current->ambiguous)
			result = handle_ambiguous_redirect(current);
		else
			result = 0;
		*file = current;
		if (result == -1 || result == -2)
			return (result);
		current = current->next;
	}
	return (0);
}
