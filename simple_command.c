#include "minishell.h"

void	exit_with_error(const char *error_msg)
{
	perror(error_msg);
    gc_free_all();
	exit(1);
}

static int	handle_child_process(t_shell *shell, t_data *data)
{
	char	*cmd_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (data->file && handle_redirections(data->file) == -1)
	{
		write(STDERR_FILENO, "bash: ", 6);
		write(STDERR_FILENO, data->file->file_name,
			ft_strlen(data->file->file_name));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		gc_free_all();
		exit(1);
	}
	cmd_path = find_command(shell, data->cmd[0]);
	if (!cmd_path)
	{
		write(STDERR_FILENO, data->cmd[0], ft_strlen(data->cmd[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		gc_free_all();
		exit(127);
	}
	ft_setenv(shell, "_", cmd_path, 1);
	execve(cmd_path, data->cmd, shell->env);
    exit_with_error("bash");
}

int	execute_command(t_shell *shell, t_data *data)
{
	pid_t	pid;
	int		status;

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