/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:29:00 by elcid             #+#    #+#             */
/*   Updated: 2024/09/21 12:48:18 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_pwd(t_shell *shell)
{
	if (shell->cwd[0] != '\0')
	{
		write(1, shell->cwd, strlen(shell->cwd));
		write(1, "\n", 1);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

int	update_pwd_env(t_shell *shell, char *old_pwd)
{
	char	*path;
	char	**env_copy;

	env_copy = shell->env;
	while (*env_copy && strncmp(*env_copy, "PWD=", 4) != 0)
		env_copy++;
	if (*env_copy)
		strcpy(old_pwd, *env_copy + 4);
	path = getcwd(shell->cwd, sizeof(shell->cwd));
	if (path)
	{
		ft_setenv(shell, "OLDPWD", old_pwd, 1);
		ft_setenv(shell, "PWD", shell->cwd, 1);
		return (0);
	}
	perror("getcwd");
	return (1);
}

int	builtin_cd(t_shell *shell, t_data *data)
{
	char	*path;
	char	old_pwd[MAX_PATH];

	if (data->cmd[1] && data->cmd[1][0] == '\0')
		return (0);
	if (data->cmd[1] == NULL)
	{
		path = getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else if (data->cmd[2] != NULL)
		return (write(2, "cd: too many arguments\n", 24) && 1);
	else
		path = data->cmd[1];
	if (chdir(path) != 0)
	{
		printf("bash: cd: %s: %s\n", path, "No such file or directory");
		return (1);
	}
	return (update_pwd_env(shell, old_pwd));
}
