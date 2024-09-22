/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 11:51:28 by elcid             #+#    #+#             */
/*   Updated: 2024/09/22 19:46:33 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_external_cmd(t_shell *shell, t_data *current)
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
	handle_execve_error(cmd_path);
}

void	exec_child_process(t_shell *shell, t_data *current)
{
	int	exit_code;

	g_global.is_main_shell = 0;
	exit_code = 0;
	if (current->file && handle_redirections(&current->file) == -1)
	{
		print_error_not_such_file(current->file->file_name);
		exit(1);
	}
	if (!current->cmd || current->cmd[0] == NULL)
	{
		gc_free_all();
		exit(0);
	}
	if (is_builtin(current->cmd[0]))
	{
		exit_code = execute_builtin(shell, current);
		gc_free_all();
		exit(exit_code);
	}
	else
		exec_external_cmd(shell, current);
}

static int	wait_for_children(pid_t *pids, int cmd_count)
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
			if ((status & 127) == 2)
				return (write(1, "\n", 1), 130);
			else if ((status & 127) == 3)
				return (write(1, "Quit (core dumped)\n", 20), 131);
			else
				last_status = status >> 8;
		}
		i++;
	}
	return (last_status);
}

void	fork_and_execute(t_shell *shell, t_data *data, int **pipes, pid_t *pids)
{
	int		i;
	int		cmd_count;
	t_data	*current;

	i = 0;
	cmd_count = count_commands(data);
	current = data;
	while (i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			setup_child_pipes(pipes, i, cmd_count);
			exec_child_process(shell, current);
		}
		else if (pids[i] < 0)
			exit_with_error("fork");
		current = current->next;
		i++;
	}
}

int	execute_pipeline(t_shell *shell, t_data *data)
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	int		last_status;

	cmd_count = count_commands(data);
	create_pipes(&pipes, cmd_count);
	pids = gc_malloc(cmd_count * sizeof(pid_t));
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	fork_and_execute(shell, data, pipes, pids);
	close_pipes(pipes, cmd_count);
	last_status = wait_for_children(pids, cmd_count);
	signal(SIGINT, sigint_handler);
	return (last_status);
}
