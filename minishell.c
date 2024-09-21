/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 22:38:05 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/21 19:32:25 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global = {NULL, 0, 0, 1};

int	execute_data(t_shell *shell, t_data *data)
{
	if (ft_herdoc(data, shell->env))
		return (0);
	handle_command(shell, data);
	return (1);
}

int	process_input_line(t_shell *shell, char *input)
{
	t_data	*data;

	if (!validate_quotes(input))
	{
		printf("syntax error\n");
		g_global.exit_number = 2;
		return (0);
	}
	if (g_global.signal_received)
		g_global.exit_number = 130;
	if (*input)
	{
		add_history(input);
		data = convert_input_to_data(input, shell);
		if (data && !execute_data(shell, data))
			return (0);
		free_data(data);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;

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
		if (!process_input_line(&shell, input))
		{
			gc_remove_ptr(input);
			continue ;
		}
		gc_remove_ptr(input);
	}
	cleanup_shell();
	return (g_global.exit_number);
}
