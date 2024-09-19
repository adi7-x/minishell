/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:49:51 by elcid             #+#    #+#             */
/*   Updated: 2024/09/18 19:49:23 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
