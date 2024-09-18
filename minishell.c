
#include "minishell.h"

t_global	g_global = {NULL, 0, 0, 1};

char	*strjoin(char *s1, char *s2, char *delimiter)
{
	int		len2;
	int		len1;
	int		len_delim;
	char	*result;

	if (s2 != NULL)
		len2 = strlen(s2);
	else
		len2 = 0;
	len1 = strlen(s1);
	len_delim = strlen(delimiter);
	result = gc_malloc(len1 + len2 + len_delim + 1);
	if (!result)
		return (NULL);
	strcpy(result, s1);
	strcat(result, delimiter);
	if (s2)
		strcat(result, s2);
	return (result);
}

int	is_builtin(char *cmd)
{
	char	*builtins[10] = {"echo", "cd", "pwd", "export", "unset", "env",
			"exit", ".", NULL};
	int		i;

	i = 0;
	while (builtins[i])
	{
		if (strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	builtin_echo(t_data *data)
{
	int	i;
	int	n_flag;
	int	j;

	i = 1;
	n_flag = 0;
	while (data->cmd[i] && strncmp(data->cmd[i], "-n", 2) == 0)
	{
		j = 2;
		while (data->cmd[i][j] == 'n')
			j++;
		if (data->cmd[i][j] == '\0')
		{
			n_flag = 1;
			i++;
		}
		else
			break ;
	}
	while (data->cmd[i])
	{
		write(1, data->cmd[i], strlen(data->cmd[i]));
		if (data->cmd[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}

long long	custom_atoi(const char *str, int *error)
{
	long long	result;
	int			sign;
	int			digit;
	long long	new_result;

	result = 0;
	sign = 1;
	*error = 0;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
		{
			*error = 1;
			return (0);
		}
		digit = *str - '0';
		new_result = result * 10 + digit;
		if (new_result < result)
		{
			*error = 2;
			return (0);
		}
		result = new_result;
		str++;
	}
	return (sign * result);
}

int	builtin_exit(t_data *data)
{
	long long	exit_code;
	int			error;

	exit_code = 0;
	error = 0;
	if (data->cmd[1])
	{
		exit_code = custom_atoi(data->cmd[1], &error);
		if (error == 1 || error == 2)
		{
			printf("bash: exit: %s: numeric argument required\n", data->cmd[1]);
			exit_code = 2;
		}
		else if (data->cmd[2])
			return (write(2, "bash: exit: too many arguments\n", 31) && 1);
	}
	if (g_global.is_main_shell)
	{
		printf("exit\n");
		cleanup();
		exit((int)(exit_code & 255));
	}
	else
		return (int)(exit_code & 255);
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
	{
		if (!data->cmd[1])
		{
			write(STDERR_FILENO,
				"bash: .: filename argument required\n.: usage: . filename [arguments]\n",
				70);
			return (2);
		}
	}
	return (1);
}

int	handle_redirections(t_file *file)
{
	int		fd;
	t_file	*current;

	current = file;
	while (current)
	{
		if (current->heredoc)
		{
			// For heredoc, we need to read from the pipe
			dup2(current->fd[0], STDIN_FILENO);
			close(current->fd[0]);
			close(current->fd[1]); // Make sure to close the write end
		}
		if (current->infile)
		{
			fd = open(current->file_name, O_RDONLY);
			if (fd == -1)
				return (-1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (current->outfile)
		{
			fd = open(current->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (-1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->append)
		{
			fd = open(current->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (-1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		current = current->next;
	}
	return (0);
}

char	*ft_strtok(char *str, char sepa)
{
	static char	*stock = NULL;
	char		*ptr;

	if (str != NULL)
	{
		if (stock != NULL)
			gc_remove_ptr(stock);
		stock = gc_strdup(str);
	}
	if (stock == NULL)
		return (NULL);
	if (*stock == sepa)
		stock++;
	if (*stock == '\0')
		return (NULL);
	ptr = stock;
	while (*stock != '\0' && *stock != sepa)
		stock++;
	if (*stock == sepa)
	{
		*stock = '\0';
		stock++;
	}
	return (ptr);
}

char	**copy_env(char **envp, t_shell *shell)
{
	char	**new_env;
	char	cwd[MAX_PATH];

	if (!envp || !*envp)
	{
		new_env = gc_malloc(sizeof(char *) * 5);
		if (!new_env)
			return (NULL);
		getcwd(cwd, sizeof(cwd));
		new_env[0] = strjoin("PWD=", cwd, "");
		new_env[1] = gc_strdup("SHLVL=1");
		new_env[2] = gc_strdup("_=/usr/bin/env");
		new_env[3] = gc_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
		new_env[4] = NULL;
		shell->ignore_path = 1;
		return (new_env);
	}
	int i, count = 0;
	while (envp[count])
		count++;
	new_env = gc_malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = gc_strdup(envp[i]);
		i++;
	}
	new_env[count] = NULL;
	shell->ignore_path = 0;
	return (new_env);
}
static int	handle_builtin(t_shell *shell, t_data *data)
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

static int	execute_single_command(t_shell *shell, t_data *data)
{
	int	status;

	if (data->cmd && data->cmd[0] && is_builtin(data->cmd[0]))
	{
		status = handle_builtin(shell, data);
		if (strcmp(data->cmd[0], "exit") == 0 && status != 1)
		{
			cleanup();
			exit(status);
		}
	}
	else
		status = execute_command(shell, data);
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
char	*int_to_str(int nbr)
{
	char	*str;
	int		len;
	int		n;

	len = 1;
	n = nbr;
	while (n /= 10)
		len++;
	str = gc_malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = nbr % 10 + '0';
		nbr /= 10;
	}
	return (str);
}

void	initialize_shell(t_shell *shell, char **envp)
{
	char	*shlvl;
	int		level;
	char	*new_shlvl;

	shell->env = copy_env(envp, shell);
	g_global.exit_number = 0;
	if (getcwd(shell->cwd, sizeof(shell->cwd)) == NULL)
	{
		perror("getcwd");
		exit(1);
	}
	shlvl = ft_getenv(shell->env, "SHLVL");
	if (shlvl)
	{
		level = atoi(shlvl) + 1;
		new_shlvl = int_to_str(level);
		if (new_shlvl)
		{
			ft_setenv(shell, "SHLVL", new_shlvl, 1);
			gc_remove_ptr(new_shlvl);
		}
	}
	else
		ft_setenv(shell, "SHLVL", "1", 1);
	setup_signals();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	t_data	*data;

	(void)argc;
	(void)argv;
	initialize_shell(&shell, envp);
	while (1)
	{
		g_global.signal_received = 0;
		input = readline("\033[1;33mminishell> \033[0m");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (!check_quotes(input))
		{
			printf("syntax error\n");
			gc_remove_ptr(input);
			continue ;
		}
		if (g_global.signal_received)
			g_global.exit_number = 130;
		if (*input)
		{
			add_history(input);
			data = process_input(input, &shell);
			if (data)
			{
				if (ft_herdoc(data, shell.env))
				{
					free_data(data);
					gc_remove_ptr(input);
					// continue ;
				}
				// this line for ambuguous:
				if (check_file1(data) == 1)
				{
					free_data(data);
					gc_remove_ptr(input);
					continue ;
				}
				handle_command(&shell, data);
				free_data(data);
			}
		}
		gc_remove_ptr(input);
		// printf("exit_number: %d\n", g_global.exit_number);
	}
	cleanup();
	return (g_global.exit_number);
}
