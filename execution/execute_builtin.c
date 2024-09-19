/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:54:31 by elcid             #+#    #+#             */
/*   Updated: 2024/09/19 13:04:47 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	char	*builtins[9];
	int		i;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = ".";
	builtins[8] = NULL;
	i = 0;
	while (builtins[i])
	{
		if (strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	builtin_dot(t_data *data)
{
	if (!data->cmd[1])
	{
		write(STDERR_FILENO,
			"bash: .: filename argument required\n"
			".: usage: . filename [arguments]\n",
			70);
	}
	return (2);
}

int	execute_builtin(t_shell *shell, t_data *data)
{
	if (data->file && data->file->file_name)
		handle_redirections(data->file);
	if (strcmp(data->cmd[0], "echo") == 0)
		return (builtin_echo(data));
	else if (strcmp(data->cmd[0], "cd") == 0)
		return (builtin_cd(shell, data));
	else if (strcmp(data->cmd[0], "pwd") == 0)
		return (builtin_pwd(shell));
	else if (strcmp(data->cmd[0], "export") == 0)
		return (builtin_export(shell, data));
	else if (strcmp(data->cmd[0], "unset") == 0)
		return (builtin_unset(shell, data));
	else if (strcmp(data->cmd[0], "env") == 0)
		return (builtin_env(shell));
	else if (strcmp(data->cmd[0], "exit") == 0)
		return (builtin_exit(data));
	else if (strcmp(data->cmd[0], ".") == 0)
		return (builtin_dot(data));
	return (1);
}
