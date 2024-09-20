/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 22:25:07 by elcid             #+#    #+#             */
/*   Updated: 2024/09/19 22:37:13 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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