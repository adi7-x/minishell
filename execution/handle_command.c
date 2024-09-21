/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:04:55 by elcid             #+#    #+#             */
/*   Updated: 2024/09/21 18:53:02 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_builtin(t_shell *shell, t_data *data)
{
	int	status;
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (handle_redirections(data->file) == -1)
	{
		write(STDERR_FILENO, "bash: ", 6);
		write(STDERR_FILENO, data->file->file_name,
			strlen(data->file->file_name));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (1);
	}
	status = execute_builtin(shell, data);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	return (status);
}

int	execute_single_command(t_shell *shell, t_data *data)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	if (data->cmd && data->cmd[0] && is_builtin(data->cmd[0]))
		status = handle_builtin(shell, data);
	else
		status = execute_command(shell, data);
	signal(SIGINT, sigint_handler);
	return (status);
}

void	handle_command(t_shell *shell, t_data *data)
{
	int	status;

	if (!data)
		return ;
	if (data->next)
		status = execute_pipeline(shell, data);
	else
		status = execute_single_command(shell, data);
	g_global.exit_number = status;
}
