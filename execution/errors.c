/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 22:25:07 by elcid             #+#    #+#             */
/*   Updated: 2024/09/22 15:58:03 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_ambiguous_redirect(t_file *new)
{
	if (new->ambiguous == 1)
	{
		write(2, "bash:", 6);
		write(2, new->file_name, ft_strlen(new->file_name));
		write(2, ": ambiguous redirect\n", 21);
		gc_free_all();
		exit(1);
	}
	return (0);
}

void	handle_execve_error(char *path)
{
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
		{
			write(2, path, ft_strlen(path));
			write(2, ": is a directory\n", 18);
			gc_free_all();
			exit(126);
		}
		else
		{
			write(2, path, ft_strlen(path));
			write(2, ": Permission denied\n", 20);
			exit(126);
		}
	}
	perror(path);
	gc_free_all();
	exit(127);
}

void	print_error_not_such_file(char *file_name)
{
	write(STDERR_FILENO, "bash: ", 6);
	write(STDERR_FILENO, file_name, strlen(file_name));
	write(STDERR_FILENO, ": No such file or directory\n", 28);
	gc_free_all();
}

void	exit_with_error(char *error_msg)
{
	perror(error_msg);
	gc_free_all();
	exit(1);
}
