/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_v1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:21:05 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/20 09:54:42 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_to_data(t_data **data, t_file **file, char ***cmd)
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

t_data	*ft_parsing(t_lexer *lexer, char **envp)
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
			var.cmd = ft_addstring(var.cmd, lexer, envp);
		else if (lexer->type == TOKEN_WORD && lexer->prev->type != TOKEN_WORD
			&& lexer->prev->type != TOKEN_PIPE)
			append_to_file(lexer, lexer->prev->type, &var.file, envp);
		else if (lexer->type == TOKEN_PIPE)
			append_to_data(&data, &var.file, &var.cmd);
		else if (lexer->type == TOKEN_WORD)
			var.cmd = ft_addstring(var.cmd, lexer, envp);
		lexer = lexer->next;
	}
	append_to_data(&data, &var.file, &var.cmd);
	free_lexer(var.lexer);
	return (data);
}

t_data	*process_input(char *input, t_shell *shell)
{
	t_lexer	*lexer_output;
	t_data	*data;

	data = NULL;
	lexer_output = NULL;
	lexer_analysis(input, &lexer_output);
	if (!lexer_output)
		return (NULL);
	data = ft_parsing(lexer_output, shell->env);
	if (!data)
	{
		free_lexer(lexer_output);
		return (NULL);
	}
	return (data);
}
