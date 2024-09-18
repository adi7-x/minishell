/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 11:51:28 by elcid             #+#    #+#             */
/*   Updated: 2024/09/18 16:16:27 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_with_error(char *error_msg)
{
	perror(error_msg);
	gc_free_all();
	exit(1);
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

void	execute_external_command(t_shell *shell, t_data *current)
{
	char	*cmd_path;

	cmd_path = find_command(shell, current->cmd[0]);
	if (!cmd_path)
	{
		write(STDERR_FILENO, "bash: ", 6);
		write(STDERR_FILENO, current->cmd[0], ft_strlen(current->cmd[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		gc_free_all();
		exit(127);
	}
	execve(cmd_path, current->cmd, shell->env);
	ft_handle_execve_error(cmd_path);
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

void	setup_child_pipes(int **pipes, int i, int cmd_count)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_pipes(pipes, cmd_count);
}

void	execute_child_process(t_shell *shell, t_data *current)
{
	g_global.is_main_shell = 0;
	if (current->file && handle_redirections(current->file) == -1)
	{
		write(STDERR_FILENO, "bash: ", 6);
		write(STDERR_FILENO, current->file->file_name,
			ft_strlen(current->file->file_name));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		gc_free_all();
		exit(1);
	}
	if (!current->cmd)
	{
		gc_free_all();
		exit(0);
	}
	if (is_builtin(current->cmd[0]))
		exit(execute_builtin(shell, current));
	else
		execute_external_command(shell, current);
}

int	wait_for_children(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}

int	execute_pipeline(t_shell *shell, t_data *data)
{
	int cmd_count;
	int **pipes;
	pid_t *pids;
	t_data *current;
	int i;

	cmd_count = count_commands_and_create_pipes(data, &pipes);
	pids = gc_malloc(cmd_count * sizeof(pid_t));
	current = data;
	i = 0;
	while (i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			setup_child_pipes(pipes, i, cmd_count);
			execute_child_process(shell, current);
		}
		else if (pids[i] < 0)
			exit_with_error("fork");
		current = current->next;
		i++;
	}
	close_pipes(pipes, cmd_count);
	return (wait_for_children(pids, cmd_count));
}