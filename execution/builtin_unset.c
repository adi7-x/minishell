/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:34:35 by elcid             #+#    #+#             */
/*   Updated: 2024/09/20 17:26:41 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
