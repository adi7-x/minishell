/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_parsing_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:27:15 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 15:58:03 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_parsed_data(t_data **data, t_file **file, char ***cmd)
{
	t_data	*newdata;
	t_data	*tmp;

	newdata = ft_calloc(sizeof(t_data), 1);
	if (newdata == NULL)
		return ;
	newdata->cmd = *cmd;
	newdata->file = *file;
	*file = NULL;
	tmp = *data;
	if (*data)
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = newdata;
	}
	else
		*data = newdata;
	*cmd = NULL;
}

t_data	*parse_lexer_to_data(t_lexer *lexer, char **envp)
{
	t_data	*data;
	t_var	var;

	var.lexer = lexer;
	var.cmd = NULL;
	data = NULL;
	if (lexer == NULL)
		return (NULL);
	var.file = NULL;
	while (lexer)
	{
		if (lexer->type == TOKEN_WORD && lexer->prev == NULL)
			var.cmd = add_command_string(var.cmd, lexer, envp);
		else if (lexer->type == TOKEN_WORD && lexer->prev->type != TOKEN_WORD
			&& lexer->prev->type != TOKEN_PIPE)
			handle_file_redirection(lexer, lexer->prev->type, &var.file, envp);
		else if (lexer->type == TOKEN_PIPE)
			add_parsed_data(&data, &var.file, &var.cmd);
		else if (lexer->type == TOKEN_WORD)
			var.cmd = add_command_string(var.cmd, lexer, envp);
		lexer = lexer->next;
	}
	add_parsed_data(&data, &var.file, &var.cmd);
	free_lexer(var.lexer);
	return (data);
}

t_data	*convert_input_to_data(char *input, t_shell *shell)
{
	t_lexer	*lexer_output;
	t_data	*data;

	data = NULL;
	lexer_output = NULL;
	lexer_analysis(input, &lexer_output);
	if (!lexer_output)
		return (NULL);
	data = parse_lexer_to_data(lexer_output, shell->env);
	if (!data)
	{
		free_lexer(lexer_output);
		return (NULL);
	}
	return (data);
}
