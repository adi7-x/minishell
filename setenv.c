

#include "minishell.h"

char	*create_env_entry(char *key, char *value)
{
	if (value)
		return (strjoin(key, value, "="));
	return (strjoin(key, "", ""));
}
int	handle_append_mode(t_shell *shell, char *key, char *value, int index)
{
	char	*old_value;
	char	*new_value;

	old_value = ft_strchr(shell->env[index], '=');
	if (old_value)
	{
		old_value++;
		new_value = strjoin(old_value, value, "");
		shell->env[index] = create_env_entry(key, new_value);
		gc_remove_ptr(new_value);
	}
	else
		shell->env[index] = create_env_entry(key, value);
	return (0);
}

int	update_existing_env(t_shell *shell, char *key, char *value,
		int overwrite)
{
	int		index;
	int		key_len;

	index = is_inenv(shell->env, key);
	if (index >= 0)
	{
		key_len = ft_strlen(key);
		if (overwrite == 1)
			shell->env[index] = create_env_entry(key, value);
		else if (overwrite == 2)
			return (handle_append_mode(shell, key, value, index));
		else if (overwrite == 3)
		{
			if (key[key_len - 1] == '+')
			{
				key[key_len - 1] = '\0';
				return (handle_append_mode(shell, key, value, index));
			}
			else
				shell->env[index] = create_env_entry(key, value);
		}
		return (0);
	}
	return (1);
}

int	ft_setenv(t_shell *shell, char *key, char *value, int overwrite)
{
	int		env_size;
	char	**new_env;
    int		i;

	if (ft_strncmp(key, "PATH", 4) == 0)
		shell->ignore_path = 0;
	if (update_existing_env(shell, key, value, overwrite) == 0)
		return (0);
	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	new_env = gc_malloc(sizeof(char *) * (env_size + 2));
	if (!new_env)
		return (-1);
	i = 0;
	while (i < env_size)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[env_size] = create_env_entry(key, value);
	new_env[env_size + 1] = NULL;
	shell->env = new_env;
	return (0);
}

// 001 https://claude.ai/chat/348a0c56-8ff6-4132-b0e9-8c8e2dfe565e