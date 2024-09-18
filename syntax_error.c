/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:08:15 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/18 16:30:35 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <string.h>

int	syntax_error(t_lexer *lexer)
{
	while (lexer->next)
	{
		if (lexer->type == TOKEN_PIPE && (lexer->prev == NULL
				|| strlen(lexer->data) > 1))
			return (1);
		else if (lexer->type != TOKEN_WORD && strlen(lexer->data) > 2)
			return (1);
		lexer = lexer->next;
		if (lexer->type != TOKEN_WORD && lexer->prev->type != TOKEN_WORD
			&& lexer->prev->type != TOKEN_PIPE)
			return (1);
		if (lexer->type == TOKEN_PIPE && lexer->prev->type != TOKEN_WORD)
			return (1);
	}
	if (lexer->type != TOKEN_WORD && strlen(lexer->data) >= 2)
		return (1);
	else if (lexer->type != TOKEN_WORD)
		return (1);
	return (0);
}

int	check_syntax_errors(t_lexer **lexer)
{
	int ret;

	if (!*lexer)
	{
		return (1);
	}
	ret = syntax_error(*lexer);
	if (ret != 0)
	{
		free_lexer(*lexer);
		*lexer = NULL;
		printf("Syntax error\n");
		g_global.exit_number=2;
		return (1);
	}
	return (0);
}