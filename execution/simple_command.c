/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 10:36:16 by elcid             #+#    #+#             */
/*   Updated: 2024/09/18 17:49:57 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_handle_execve_error(char *path)
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


static char	*search_in_path(char *cmd, char *path)
{
	char	*dir;
	char	*full_path;
	char	*path_copy;

	path_copy = gc_strdup(path);
	dir = ft_strtok(path_copy, ':');
	while (dir)
	{
		full_path = strjoin(dir, cmd, "/");
		if (access(full_path, X_OK) == 0)
		{
			gc_remove_ptr(path_copy);
			return (full_path);
		}
		gc_remove_ptr(full_path);
		dir = ft_strtok(NULL, ':');
	}
	gc_remove_ptr(path_copy);
	return (NULL);
}

char	*find_command(t_shell *shell, char *cmd)
{
	char	*path;
	char	*full_path;
	int		i;

	if (strchr(cmd, '/'))
		return (gc_strdup(cmd));
	i = 0;
	while (shell->env[i] && strncmp(shell->env[i], "PATH=", 5) != 0)
		i++;
	if (shell->env[i] && strncmp(shell->env[i], "PATH=", 5) == 0)
		path = shell->env[i] + 5;
	else
		path = NULL;
	if (!path || *path == '\0')
	{
		full_path = strjoin("./", cmd, "");
		if (access(full_path, X_OK) == 0)
			return (full_path);
		gc_remove_ptr(full_path);
		return (NULL);
	}
	return (search_in_path(cmd, path));
}

void	handle_child_process(t_shell *shell, t_data *data)
{
	char	*cmd_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (data->file && handle_redirections(data->file) == -1)
	{
		write(STDERR_FILENO, "bash: ", 6);
		write(STDERR_FILENO, data->file->file_name,
			strlen(data->file->file_name));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		gc_free_all();
		exit(1);
	}
	cmd_path = find_command(shell, data->cmd[0]);
	if (!cmd_path)
	{
		write(STDERR_FILENO, data->cmd[0], strlen(data->cmd[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		gc_free_all();
		exit(127);
	}
	ft_setenv(shell, "_", cmd_path, 1);
	execve(cmd_path, data->cmd, shell->env);
	ft_handle_execve_error(cmd_path);
}

int	execute_command(t_shell *shell, t_data *data)
{
	pid_t pid;
	int status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	else if (pid == 0)
		handle_child_process(shell, data);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				return (write(1, "\n", 1), 130);
			else if (WTERMSIG(status) == SIGQUIT)
				return (write(1, "Quit (core dumped)\n", 20), 131);
		}
		status = WEXITSTATUS(status);
		signal(SIGINT, sigint_handler);
	}
	return (status);
}