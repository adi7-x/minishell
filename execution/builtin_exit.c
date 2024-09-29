/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:23:25 by elcid             #+#    #+#             */
/*   Updated: 2024/09/29 11:35:19 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_shell(void)
{
	printf("exit\n");
	cleanup_shell();
}

long long	custom_atoi(char *str, int *error)
{
	long long	result;
	int			sign;
	int			digit;

	result = 0;
	sign = 1;
	*error = 0;
	while(*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' && str++)
		sign = -1;
	else if (*str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (*error = 1, 0);
		digit = *str - '0';
		if (result > (LLONG_MAX - digit) / 10)
			return (*error = 2, 0);
		result = result * 10 + digit;
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
			return ((write(2, "bash: exit: too many arguments\n", 31) && 1));
	}
	else
		exit_code = g_global.exit_number;
	if (g_global.is_main_shell)
	{
		exit_shell();
		exit(exit_code);
	}
	else
		return (exit_code);
}
