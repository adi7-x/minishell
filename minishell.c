
#include "minishell.h"

t_global	g_global = {NULL, 0, 0};

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

int	custom_strcmp(char *a, char *b)
{
	const char	*a_end = strchr(a, '=');
	const char	*b_end = strchr(b, '=');
	int			a_len;
	int			b_len;
	int			min_len;
	int			cmp;

	if (a_end)
		a_len = (int)(a_end - a);
	else
		a_len = strlen(a);
	if (b_end)
		b_len = (int)(b_end - b);
	else
		b_len = strlen(b);
	min_len = a_len;
	if (b_len < a_len)
		min_len = b_len;
	cmp = strncmp(a, b, min_len);
	if (cmp == 0)
	{
		if (a_len != b_len)
			return (a_len - b_len);
		else
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

void	sort_export(t_shell *shell)
{
	int		i;
	int		env_count;
	char	**env_copy;
	char	*key;
	char	*val;

	i = 0;
	env_count = 0;
	while (shell->env[env_count])
		env_count++;
	env_copy = gc_malloc((env_count + 1) * sizeof(char *));
	if (!env_copy)
	{
		perror("gc_malloc");
		return ;
	}
	i = 0;
	while (i < env_count)
	{
		env_copy[i] = gc_strdup(shell->env[i]);
		if (!env_copy[i])
		{
			perror("gc_strdup");
			while (i > 0)
			{
				free(env_copy[--i]);
			}
			free(env_copy);
			return ;
		}
		i++;
	}
	env_copy[env_count] = NULL;
	bubble_sort(env_copy, env_count);
	i = 0;
	while (i < env_count)
	{
		key = gc_strdup(env_copy[i]);
		if (!key)
		{
			perror("gc_strdup");
			break ;
		}
		val = strchr(key, '=');
		if (shell->ignore_path && strncmp(key, "PATH=", 5) == 0)
		{
			i++;
			continue ;
		}
		if (val)
		{
			*val = '\0';
			val++;
			printf("declare -x %s=\"%s\"\n", key, val);
		}
		else
		{
			printf("declare -x %s\n", key);
		}
		free(key);
		free(env_copy[i]);
		i++;
	}
	free(env_copy);
}

int	builtin_export(t_shell *shell, t_data *data)
{
	int		i;
	char	*name;
	char	*value;
	char	*pluscase;

	if (!data->cmd[1])
	{
		sort_export(shell);
		return (0);
	}
	i = 1;
	while (data->cmd[i])
	{
		name = gc_strdup(data->cmd[i]);
		value = strchr(name, '=');
		pluscase = strchr(name, '+');
		if (pluscase && *(pluscase + 1) != '=')
		{
			printf("bash: export: `%s': not a valid identifier\n",
				data->cmd[i]);
			free(name);
			g_global.exit_number = 1;
		}
		else
		{
			if (value)
			{
				*value = '\0';
				value++;
			}
			if (!is_valid_identifier(name))
			{
				printf("bash: export: `%s': not a valid identifier\n",
					data->cmd[i]);
			}
			else
			{
				if (value && value[0] != '\0')
				{
					if (name[strlen(name) - 1] == '+')
					{
						printf("there is key and value\n");
						name[strlen(name) - 1] = '\0';
						ft_setenv(shell, name, value, 2);
					}
					else
					{
						printf("just =\n");
						ft_setenv(shell, name, value, 1);
					}
				}
				else if (!is_inenv(shell->env, name))
				{
					printf("not in the env\n");
					if (name[strlen(name) - 1] == '+')
						name[strlen(name) - 1] = '\0';
					ft_setenv(shell, name, value, 0);
				}
				else if (is_inenv(shell->env, name) && value
					&& value[0] == '\0')
				{
					printf("already in the env and the value is empty\n");
					ft_setenv(shell, name, value, 3);
				}
				else
				{
					printf("already in the env and without = \n");
					ft_setenv(shell, name, value, 2);
				}
			}
		}
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
		if (strchr(*env, '=') && shell->ignore_path && strncmp(*env, "PATH=",
				5) == 0)
		{
			env++;
			continue ;
		}
		write(1, *env, strlen(*env));
		write(1, "\n", 1);
		env++;
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	char	*builtins[10] = {"echo", "cd", "pwd", "export", "unset", "env",
			"exit", NULL};
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

int	builtin_cd(t_shell *shell, t_data *data)
{
	char	*path;
	char	old_pwd[MAX_PATH];
	char	**env_copy;

	env_copy = shell->env;
	printf("data->cmd[1]: %s\n", data->cmd[1]);
	if (data->cmd[2])
	{
		write(2, "cd: too many arguments\n", 24);
		return (1);
	}
	path = data->cmd[1];
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	while (*env_copy)
	{
		if (strncmp(*env_copy, "PWD=", 4) == 0)
		{
			strcpy(old_pwd, *env_copy + 4);
			break ;
		}
		env_copy++;
	}
	// printf("old_pwd: %s\n", old_pwd);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	// printf("new_pwd: %s\n", path);
	if (getcwd(shell->cwd, sizeof(shell->cwd)) != NULL)
	{
		ft_setenv(shell, "OLDPWD", old_pwd, 1);
		ft_setenv(shell, "PWD", shell->cwd, 1);
	}
	else
	{
		perror("getcwd");
		return (1);
	}
	return (0);
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
	printf("exit\n");
	if (data->cmd[1])
	{
		exit_code = custom_atoi(data->cmd[1], &error);
		if (error == 1)
		{
			printf("bash: exit: %s: numeric argument required\n", data->cmd[1]);
			exit(2);
		}
		else if (error == 2)
		{
			printf("bash: exit: %s: numeric argument required\n", data->cmd[1]);
			exit(2);
		}
		if (data->cmd[2])
		{
			printf("bash: exit: too many arguments\n");
			return (-1);
		}
	}
	exit((int)(exit_code & 255));
}
void		handle_redirections(t_file *file);
int	execute_builtin(t_shell *shell, t_data *data)
{
	if (data->file && data->file->file_name)
	{
		handle_redirections(data->file);
	}
	if (strcmp(data->cmd[0], "echo") == 0)
		return (builtin_echo(data));
	else if (strcmp(data->cmd[0], "cd") == 0)
		return (builtin_cd(shell, data));
	else if (strcmp(data->cmd[0], "pwd") == 0)
		return (builtin_pwd(shell)); // Pass shell here
	else if (strcmp(data->cmd[0], "export") == 0)
		return (builtin_export(shell, data));
	else if (strcmp(data->cmd[0], "unset") == 0)
		return (builtin_unset(shell, data));
	else if (strcmp(data->cmd[0], "env") == 0)
		return (builtin_env(shell));
	else if (strcmp(data->cmd[0], "exit") == 0)
		return (builtin_exit(data));
	return (1);
}
// t_file *create_file_node(char *filename, int type)
// {
//     t_file *new_file = gc_malloc(sizeof(t_file));
//     if (!new_file)
//         return (NULL);
//     new_file->file_name = gc_strdup(filename);
//     new_file->infile = 0;
//     new_file->outfile = 0;
//     new_file->herdoc = 0;
//     new_file->apend = 0;

//     if (type == 0)
//         new_file->infile = 1;
//     else if (type == 1)
//         new_file->outfile = 1;
//     else if (type == 2)
//         new_file->apend = 1;
//     else if (type == 3)
//         new_file->herdoc = 1;

//     new_file->next = NULL;
//     return (new_file);
// }

void	handle_redirections(t_file *file)
{
	int		fd;
	t_file	*current;

	current = file;
	while (current)
	{
		if (current->infile)
		{
			fd = open(current->file_name, O_RDONLY);
			if (fd == -1)
			{
				perror("bash");
				g_global.exit_number = 1;
				return ;
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (current->outfile)
		{
			fd = open(current->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("bash");
				g_global.exit_number = 1;
				return ;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->append)
		{
			fd = open(current->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("bash");
				g_global.exit_number = 1;
				return ;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->heredoc)
		{
		}
		current = current->next;
	}
}

char	*ft_strtok(char *str, char sepa)
{
	static char	*stock = NULL;
	char		*ptr;

	if (str != NULL)
		stock = gc_strdup(str);
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

char	*find_command(t_shell *shell, char *cmd)
{
	char	*path;
	char	*dir;
	char	*full_path;
	char	*path_copy;
	int		i;

	path = NULL;
	i = 0;
	if (strchr(cmd, '/'))
		return (gc_strdup(cmd));
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], "PATH=", 5) == 0)
		{
			path = shell->env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path || *path == '\0')
	{
		full_path = strjoin("./", cmd, "");
		if (access(full_path, X_OK) == 0)
			return (full_path);
		gc_remove_ptr(full_path);
		return (NULL);
	}
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

int	execute_command(t_shell *shell, t_data *data)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	int		fd;
	int		exit_status;

	if (is_builtin(data->cmd[0]))
	{
		fd = dup(1);
		exit_status = execute_builtin(shell, data);
		dup2(fd, 1);
		close(fd);
		return (exit_status);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		// Child process
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handle_redirections(data->file);
		cmd_path = find_command(shell, data->cmd[0]);
		if (!cmd_path)
		{
			write(2, "Command not found: ", 19);
			write(2, data->cmd[0], strlen(data->cmd[0]));
			write(2, "\n", 1);
			gc_free_all();
			exit(127);
		}
		ft_setenv(shell, "_", cmd_path, 1);
		execve(cmd_path, data->cmd, shell->env);
		if (errno == EACCES)
		{
			perror("bash");
			gc_free_all();
			exit(126);
		}
		perror("bash");
		gc_free_all();
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		// printf("status: %d\n", status);
		if (status == 2 || status == 131)
		{
			if (status == 2)
			{
				write(1, "\n", 1);
				g_global.exit_number = 130;
			}
			else if (status == 131)
			{
				write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
				g_global.exit_number = 131;
			}
		}
		else
		{
			g_global.exit_number = status >> 8;
		}
		signal(SIGINT, sigint_handler);
		return (g_global.exit_number);
	}
}

int	execute_pipeline(t_shell *shell, t_data *data)
{
	int		cmd_count;
	t_data	*current;
	int		**pipes;
	pid_t	*pids;
	int		i;
	int		j;
	int		exit_status;
	char	*cmd_path;

	cmd_count = 0;
	current = data;
	int status, last_status = 0;
	i = 0;
	while (current)
	{
		cmd_count++;
		current = current->next;
	}
	pipes = gc_malloc((cmd_count - 1) * sizeof(int *));
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = gc_malloc(2 * sizeof(int));
		pipe(pipes[i]);
		i++;
	}
	pids = gc_malloc(cmd_count * sizeof(pid_t));
	current = data;
	i = 0;
	while (i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			if (i > 0)
			{
				dup2(pipes[i - 1][0], STDIN_FILENO);
			}
			if (i < cmd_count - 1)
			{
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			j = 0;
			while (j < cmd_count - 1)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}
			handle_redirections(current->file);
			if (is_builtin(current->cmd[0]))
			{
				exit_status = execute_builtin(shell, current);
				gc_free_all();
				exit(exit_status);
			}
			else
			{
				cmd_path = find_command(shell, current->cmd[0]);
				if (!cmd_path)
				{
					printf("Command not found: %s\n", current->cmd[0]);
					gc_free_all();
					exit(127);
				}
				execve(cmd_path, current->cmd, shell->env);
				perror("execve");
				gc_free_all();
				exit(1);
			}
		}
		else if (pids[i] < 0)
		{
			perror("fork");
			exit(1);
		}
		current = current->next;
		i++;
	}
	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	i = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if ((status & 127) == 0)
				last_status = (status >> 8) & 255;
			else
				last_status = 128 + (status & 127);
		}
		i++;
	}
	// Clean up pipes
	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		gc_remove_ptr(pipes[i]); // Remove from garbage collector
		i++;
	}
	gc_remove_ptr(pipes); // Remove the array of pipes from garbage collector
	gc_remove_ptr(pids);  // Remove pids array from garbage collector
	return (last_status);
}

char	**copy_env(char **envp, t_shell *shell)
{
	char	**new_env;
	char	cwd[MAX_PATH];

	// char	**new_env;
	if (!envp || !*envp)
	{
		new_env = gc_malloc(sizeof(char *) * 5);
		if (!new_env)
			return (NULL);
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			new_env[0] = strjoin("PWD=", cwd, "");
		else
			new_env[0] = gc_strdup("PWD=/");
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
	return (new_env);
}

void	handle_command(t_shell *shell, t_data *data)
{
	int	stdin_backup;
	int	stdout_backup;
	int	exit_status;

	if (data)
	{
		if (data->next)
		{
			g_global.exit_number = execute_pipeline(shell, data);
		}
		else
		{
			stdin_backup = -1;
			stdout_backup = -1;
			if (data->file)
			{
				// Backup standard input and output
				stdin_backup = dup(STDIN_FILENO);
				stdout_backup = dup(STDOUT_FILENO);
				handle_redirections(data->file);
			}
			if (data->cmd && data->cmd[0])
			{
				if (strcmp(data->cmd[0], "exit") == 0)
				{
					exit_status = builtin_exit(data);
					if (exit_status != -1)
					{
						if (data->cmd[1])
							g_global.exit_number = atoi(data->cmd[1]);
						cleanup();
						exit(g_global.exit_number);
					}
				}
				else
				{
					g_global.exit_number = execute_command(shell, data);
				}
			}
			if (stdin_backup != -1)
			{
				dup2(stdin_backup, STDIN_FILENO);
				close(stdin_backup);
			}
			if (stdout_backup != -1)
			{
				dup2(stdout_backup, STDOUT_FILENO);
				close(stdout_backup);
			}
		}
	}
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
	shlvl = getenv("SHLVL");
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
	setup_signals();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	t_data	*data;

	if (argc > 1 || !argv)
	{
		printf("just one argument\n");
		return (0);
	}
	initialize_shell(&shell, envp);
	while (1)
	{
		g_global.signal_received = 0;
		input = readline("\033[1;33mminishell> \033[0m");
		// Handle Ctrl+D to exit the shell (input == NULL).
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		// Check if the quotes are unbalanced.
		if (!check_quotes(input))
		{
			printf("syntax error\n");
			free(input);
		}
		else
		{
			// Proceed if input is valid.
			if (g_global.signal_received)
				g_global.exit_number = 130;
			if (*input) // Proceed only if input is not empty.
			{
				add_history(input); // Add input to history.
				data = parse_input(input, &shell);
				if (data)
				{
					handle_command(&shell, data); // Execute the command.
					free_data(data);              // Free command data.
				}
				else
				{
					printf("syntax error\n"); // Handle parsing errors.
				}
				free_parsed_data(data); // Clean up parsed data.
			}
			free(input); // Free input string.
		}
	}
	cleanup(); // Clean up shell environment.
	return (g_global.exit_number);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell	shell;
// 	char	*input;
// 	t_data	*data;

// 	if (argc > 1 || !argv)
// 		{
// 			printf("just one ergument\n");
// 			return (0);
// 		}
// 	initialize_shell(&shell, envp);
// 	while (1)
// 	{
// 		g_global.signal_received = 0;
// 		input = readline("\033[1;33mminishell> \033[0m");
// 		if (!check_quotes(input))
// 			printf("syntax error\n");
// 		if (g_global.signal_received)
// 			g_global.exit_number = 130;
// 		if (*input)
// 		{
// 			add_history(input);
// 			data = parse_input(input, &shell);
// 			// if (data->cmd)
// 			// {
// 			//     int i = 0;
// 			//     while (data->cmd[i])
// 			//     {
// 			//         printf("cmd[%d]: %s\n", i, data->cmd[i]);
// 			//         i++;
// 			//     }
// 			// }
// 			if (data)
// 			{
// 				handle_command(&shell, data);
// 				free_data(data);
// 			}
// 			else
// 			{
// 				printf("syntax error1\n");
// 			}
// 			free_parsed_data(data);
// 		}
// 		free(input);
// 	}
// 	cleanup();
// 	return (g_global.exit_number);
// }

// int main(int argc, char **argv, char **envp)
// {
//     t_shell shell;
//     char *input;

//     (void)argc;
//     (void)argv;
//     initialize_shell(&shell, envp);

//     while (1)
//     {
//         g_global.signal_received = 0;
//         input = readline("\033[1;33mminishell> \033[0m");
//         if (!input)
//         {
//             printf("exit\n");
//             break ;
//         }
//         if (g_global.signal_received)
//               shell.exit_number = 130;
//         if (*input)
//         {
//             add_history(input);
//             handle_command(&shell, input);
//         }
//         free(input);
//     }
//     cleanup();
//     return (shell.exit_number);
// }