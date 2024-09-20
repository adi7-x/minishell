/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 11:25:00 by elcid             #+#    #+#             */
/*   Updated: 2024/09/14 11:25:01 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			return (i);
		}
		gc_remove_ptr(key);
		i++;
	}
	gc_remove_ptr(name_copy);
	return (-1);
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

void	handle_valid_identifier(t_shell *shell, char *name, char *value)
{
	if (value && value[0] != '\0')
	{
		// printf("DEBUG: Value is present and non-empty\n");
		if (name[strlen(name) - 1] == '+')
		{
			// printf("DEBUG: Append mode detected (2)\n");
			name[strlen(name) - 1] = '\0';
			ft_setenv(shell, name, value, 2);
		}
		else
		{
			// printf("DEBUG: Normal assignment mode (1)\n");
			ft_setenv(shell, name, value, 1);
		}
	}
	else if (is_inenv(shell->env, name) < 0)
	{
		// printf("DEBUG: Variable not in environment (0)\n");
		// printf("name=%s, value=%s\n", name, value);
		if (name[strlen(name) - 1] == '+')
			name[strlen(name) - 1] = '\0';
		ft_setenv(shell, name, value, 0);
	}
	else if (is_inenv(shell->env, name) >= 0 && value && value[0] == '\0')
	{
		// printf("DEBUG: Variable in environment, empty value (3)\n");
		ft_setenv(shell, name, value, 3);
	}
	else
	{
		// printf("DEBUG: Variable in environment, no value specified (2)\n");
		ft_setenv(shell, name, value, 2);
	}
}

int	process_export_arg(t_shell *shell, char *arg)
{
	char	*name;
	char	*value;
	char	*pluscase;

	name = gc_strdup(arg);
	value = strchr(name, '=');
	pluscase = strchr(name, '+');
	if (pluscase && *(pluscase + 1) != '=')
	{
		printf("bash: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	if (value)
	{
		*value = '\0';
		value++;
	}
	if (!is_valid_identifier(name))
	{
		printf("bash: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	else
		handle_valid_identifier(shell, name, value);
	return (0);
}

int	builtin_export(t_shell *shell, t_data *data)
{
	int	i;
	int	status;

	status = 0;
	if (!data->cmd[1])
	{
		sort_export(shell);
		return (0);
	}
	i = 1;
	while (data->cmd[i])
	{
		// printf("DEBUG: Processing argument %d: %s\n", i, data->cmd[i]);
		status = process_export_arg(shell, data->cmd[i]);
		i++;
	}
	return (status);
}
