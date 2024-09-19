/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 10:18:57 by elcid             #+#    #+#             */
/*   Updated: 2024/09/18 19:57:19 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	print_env_entry(char *entry, t_shell *shell)
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

void	print_sorted_env(char **env_copy, int env_count, t_shell *shell)
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

char	**copy_and_sort_env(t_shell *shell, int *env_count)
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
