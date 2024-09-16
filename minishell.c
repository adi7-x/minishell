
#include "minishell.h"

t_global	g_global = {NULL, 0, 0, 1};

void	sigint_handler(int sig)
{
	(void)sig;
	g_global.signal_received = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	free_data(t_data *data)
{
	t_data	*next;
	t_file	*file;
	t_file	*next_file;
	int		i;

	while (data)
	{
		next = data->next;
		if (data->cmd)
		{
			i = 0;
			while (data->cmd[i])
			{
				gc_remove_ptr(data->cmd[i]);
				i++;
			}
			gc_remove_ptr(data->cmd);
		}
		file = data->file;
		while (file)
		{
			next_file = file->next;
			gc_remove_ptr(file->file_name);
			gc_remove_ptr(file);
			file = next_file;
		}
		gc_remove_ptr(data);
		data = next;
	}
}
void	cleanup(void)
{
	gc_free_all();
	rl_clear_history();
}

int	ft_unsetenv(t_shell *shell, char *name)
{
	int	i;
	int	len;

	i = 0;
	len = strlen(name);
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], name, len) == 0 && (shell->env[i][len] == '='
				|| shell->env[i][len] == '\0'))
		{
			gc_remove_ptr(shell->env[i]);
			while (shell->env[i])
			{
				shell->env[i] = shell->env[i + 1];
				i++;
			}
			return (0);
		}
		i++;
	}
	return (0);
}

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

int	is_inenv(char **env, char *name)
{
	int		i;
	char	*key;
	char	*val;
	char	*name_copy;

	i = 0;
	name_copy = gc_strdup(name);
	if (strchr(name_copy, '+'))
		name_copy[strlen(name_copy) - 1] = '\0';
	while (env[i])
	{
		key = gc_strdup(env[i]);
		val = strchr(key, '=');
		if (val)
			*val = '\0';
		if (strcmp(key, name_copy) == 0)
		{
			gc_remove_ptr(key);
			gc_remove_ptr(name_copy);
			return (i);
		}
		gc_remove_ptr(key);
		i++;
	}
	gc_remove_ptr(name_copy);
	return (-1);
}

int	ft_setenv(t_shell *shell, char *key, char *value, int overwrite)
{
	int		i;
	char	*new_var;
	int		env_size;
	char	**new_env;
	int		index;
	char	*old_value;
	char	*new_value;

	i = 0;
	if (strncmp(key, "PATH", 4) == 0)
		shell->ignore_path = 0;
	while (shell->env[i])
	{
		index = is_inenv(shell->env, key);
		if (index >= 0 && overwrite)
		{
			if (overwrite == 1)
			{
				shell->env[index] = strjoin(key, value, "=");
			}
			else if (overwrite == 3)
			{
				if (key[strlen(key) - 1] == '+' && shell->env[index][strlen(key)
					- 1] != '=')
				{
					key[strlen(key) - 1] = '\0';
					shell->env[index] = strjoin(key, value, "=");
				}
				else if (key[strlen(key) - 1] == '+')
				{
					key[strlen(key) - 1] = '\0';
					return (0);
				}
				else
				{
					shell->env[index] = strjoin(key, value, "=");
				}
			}
			else if (overwrite == 2)
			{
				if (shell->env[index][strlen(key)] != '=' && (value
						&& value[0] != '\0'))
				{
					shell->env[index] = strjoin(key, value, "=");
				}
				else
				{
					old_value = strchr(shell->env[index], '=');
					if (old_value)
					{
						old_value++;
						new_value = strjoin(old_value, value, "");
						shell->env[index] = strjoin(key, new_value, "=");
					}
				}
			}
			return (0);
		}
		i++;
	}
	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	new_env = gc_malloc(sizeof(char *) * (env_size + 2));
	if (!new_env)
	{
		perror("gc_malloc");
		return (-1);
	}
	i = 0;
	while (i < env_size)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	if (value)
	{
		new_var = strjoin(key, value, "=");
	}
	else
	{
		new_var = strjoin(key, "", "");
	}
	new_env[env_size] = new_var;
	new_env[env_size + 1] = NULL;
	shell->env = new_env;
	return (0);
}

int	is_valid_identifier(const char *name)
{
	int	i;

	i = 1;
	if (!isalpha(name[0]) && name[0] != '_')
		return (0);
	while (name[i])
	{
		if (name[i] == '+' && name[i + 1] == '\0')
			break ;
		if (!isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	get_comparison_length(const char *str, int *len)
{
	const char	*end;

	end = strchr(str, '=');
	if (end)
		*len = (int)(end - str);
	else
		*len = strlen(str);
	return (*len);
}

int	custom_strcmp(char *a, char *b)
{
	int	a_len;
	int	b_len;
	int	min_len;
	int	cmp;

	get_comparison_length(a, &a_len);
	get_comparison_length(b, &b_len);
	if (a_len < b_len)
		min_len = a_len;
	else
		min_len = b_len;
	cmp = strncmp(a, b, min_len);
	if (cmp == 0)
	{
		if (a_len != b_len)
			return (a_len - b_len);
		return (strcmp(a, b));
	}
	return (cmp);
}

void	bubble_sort(char **arr, int n)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - 1 - i)
		{
			if (custom_strcmp(arr[j], arr[j + 1]) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_env_entry(char *entry, t_shell *shell)
{
	char	*key;
	char	*val;

	key = gc_strdup(entry);
	if (!key)
		return ;
	val = strchr(key, '=');
	if (shell->ignore_path && strncmp(key, "PATH=", 5) == 0)
	{
		gc_remove_ptr(key);
		return ;
	}
	if (val)
	{
		*val = '\0';
		val++;
		printf("declare -x %s=\"%s\"\n", key, val);
	}
	else
		printf("declare -x %s\n", key);
	gc_remove_ptr(key);
}

static void	print_sorted_env(char **env_copy, int env_count, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < env_count)
	{
		print_env_entry(env_copy[i], shell);
		gc_remove_ptr(env_copy[i]);
		i++;
	}
}

static char	**copy_and_sort_env(t_shell *shell, int *env_count)
{
	char	**env_copy;
	int		i;

	*env_count = 0;
	while (shell->env[*env_count])
		(*env_count)++;
	env_copy = gc_malloc((*env_count + 1) * sizeof(char *));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (i < *env_count)
	{
		env_copy[i] = gc_strdup(shell->env[i]);
		if (!env_copy[i])
		{
			while (i > 0)
				gc_remove_ptr(env_copy[--i]);
			gc_remove_ptr(env_copy);
			return (NULL);
		}
		i++;
	}
	env_copy[*env_count] = NULL;
	bubble_sort(env_copy, *env_count);
	return (env_copy);
}

void	sort_export(t_shell *shell)
{
	int		env_count;
	char	**env_copy;

	env_copy = copy_and_sort_env(shell, &env_count);
	if (!env_copy)
	{
		perror("gc_malloc");
		return ;
	}
	print_sorted_env(env_copy, env_count, shell);
	gc_remove_ptr(env_copy);
}

void	handle_valid_identifier(t_shell *shell, char *name, char *value)
{
	if (value && value[0] != '\0')
	{
		printf("DEBUG: Value is present and non-empty\n");
		if (name[strlen(name) - 1] == '+')
		{
			printf("DEBUG: Append mode detected (2)\n");
			name[strlen(name) - 1] = '\0';
			ft_setenv(shell, name, value, 2);
		}
		else
		{
			printf("DEBUG: Normal assignment mode (1)\n");
			ft_setenv(shell, name, value, 1);
		}
	}
	else if (is_inenv(shell->env, name) < 0)
	{
		printf("DEBUG: Variable not in environment (0)\n");
		if (name[strlen(name) - 1] == '+')
			name[strlen(name) - 1] = '\0';
		ft_setenv(shell, name, value, 0);
	}
	else if (is_inenv(shell->env, name) >= 0 && value && value[0] == '\0')
	{
		printf("DEBUG: Variable in environment, empty value (3)\n");
		ft_setenv(shell, name, value, 3);
	}
	else
	{
		printf("DEBUG: Variable in environment, no value specified (2)\n");
		ft_setenv(shell, name, value, 2);
	}
}

void	process_export_arg(t_shell *shell, char *arg)
{
	char	*name;
	char	*value;

	name = gc_strdup(arg);
	value = strchr(name, '=');
	if (value)
	{
		*value = '\0';
		value++;
		printf("DEBUG: Separated name=%s, value=%s\n", name, value);
	}
	if (!is_valid_identifier(name))
	{
		if (value)
			gc_remove_ptr(value);
		gc_remove_ptr(name);
		g_global.exit_number = 1;
		printf("bash: export: `%s': not a valid identifier\n", arg);
	}
	else
		handle_valid_identifier(shell, name, value);
}

int	builtin_export(t_shell *shell, t_data *data)
{
	int	i;

	if (!data->cmd[1])
	{
		sort_export(shell);
		return (0);
	}
	i = 1;
	while (data->cmd[i])
	{
		printf("DEBUG: Processing argument %d: %s\n", i, data->cmd[i]);
		process_export_arg(shell, data->cmd[i]);
		i++;
	}
	return (0);
}

int	builtin_env(t_shell *shell)
{
	char	**env;

	env = shell->env;
	while (*env)
	{
		if (strchr(*env, '='))
		{
			if (shell->ignore_path == 1 && strncmp(*env, "PATH=", 5) == 0)
			{
				env++;
				continue ;
			}
			else
			{
				write(1, *env, strlen(*env));
				write(1, "\n", 1);
			}
		}
		env++;
	}
	return (0);
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
	{
		write(2, "cd: too many arguments\n", 24);
		return (1);
	}
	else
		path = data->cmd[1];
	if (chdir(path) != 0)
	{
		printf("bash: cd: %s: %s\n", path, "No such file or directory");
		return (1);
	}
	return (update_pwd_env(shell, old_pwd));
}

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

int	builtin_unset(t_shell *shell, t_data *data)
{
	int	i;

	i = 1;
	while (data->cmd[i])
	{
		ft_unsetenv(shell, data->cmd[i]);
		i++;
	}
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
int			handle_redirections(t_file *file);
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

int	execute_command(t_shell *shell, t_data *data)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (data->file)
		{
			if (handle_redirections(data->file) == -1)
			{
				write(STDERR_FILENO, "bash: ", 6);
				write(STDERR_FILENO, data->file->file_name,
					strlen(data->file->file_name));
				write(STDERR_FILENO, ": No such file or directory\n", 28);
				gc_free_all();
				exit(1);
			}
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
		perror("bash");
		gc_free_all();
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
			{
				write(1, "\n", 1);
				status = 130;
			}
			else if (WTERMSIG(status) == SIGQUIT)
			{
				write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
				status = 131;
			}
		}
		else
		{
			status = WEXITSTATUS(status);
		}
		signal(SIGINT, sigint_handler);
		return (status);
	}
}

void	exit_with_error(const char *error_msg)
{
	perror(error_msg);
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
	perror("bash");
	gc_free_all();
	exit(1);
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
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	t_data	*current;
	int		i;

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
	{
		ft_setenv(shell, "SHLVL", "1", 1);
	}
	// Initialize var_us
	shell->var_us.envp = convert_env_to_list(shell->env);
	// Convert env to t_env list
	shell->var_us.infd = -2;
	shell->var_us.outfd = -2;
	// Initialize other fields of var_us as needed
	// For example:
	shell->var_us.id = NULL; // Assuming id is a pointer, initialize it to NULL
	shell->var_us.n = 0;     // Initialize other fields as needed
	setup_signals();
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    char *input;

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
            break;
        }
        if (!check_quotes(input))
        {
            printf("syntax error\n");
            gc_remove_ptr(input);
            continue;
        }
        if (g_global.signal_received)
            g_global.exit_number = 130;
        if (*input)
        {
            add_history(input);
            t_data *data = process_input(input, &shell);
            if (data)
            {
                if (ft_herdoc(data, shell.env))
                {
                   
                    free_data(data);
                    gc_remove_ptr(input);
                    // continue;
                }
                // this line for ambuguous:
                if (check_file1(data, &shell.var_us)== 1)
                {
                    free_data(data);
                    gc_remove_ptr(input);
                    continue;
                }
                handle_command(&shell, data);
                free_data(data);
            }
        }
        gc_remove_ptr(input);
        // printf("exit_number: %d\n", g_global.exit_number);
		gc_free_all();
    }
    return g_global.exit_number;
}
