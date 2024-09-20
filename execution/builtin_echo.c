/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:37:45 by elcid             #+#    #+#             */
/*   Updated: 2024/09/20 17:26:41 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_n_flag(char *arg)
{
	int	i;

	if (arg == NULL || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i] != '\0')
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(t_data *data)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (data->cmd[i] && check_n_flag(data->cmd[i]))
	{
		n_flag = 1;
		i++;
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
