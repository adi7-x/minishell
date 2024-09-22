/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:08:15 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 16:00:52 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	validate_syntax(t_lexer *lexer)
{
	while (lexer->next)
	{
		if (lexer->type == TOKEN_PIPE && (lexer->prev == NULL
				|| ft_strlen(lexer->data) > 1))
			return (1);
		else if (lexer->type != TOKEN_WORD && ft_strlen(lexer->data) > 2)
			return (1);
		lexer = lexer->next;
		if (lexer->type != TOKEN_WORD && lexer->prev->type != TOKEN_WORD
			&& lexer->prev->type != TOKEN_PIPE)
			return (1);
		if (lexer->type == TOKEN_PIPE && lexer->prev->type != TOKEN_WORD)
			return (1);
	}
	if (lexer->type != TOKEN_WORD && ft_strlen(lexer->data) >= 2)
		return (1);
	else if (lexer->type != TOKEN_WORD)
		return (1);
	return (0);
}

int	process_syntax_check(t_lexer **lexer)
{
	int	syntax_status;

	if (!*lexer)
		return (1);
	syntax_status = validate_syntax(*lexer);
	if (syntax_status != 0)
	{
		free_lexer(*lexer);
		*lexer = NULL;
		printf("Syntax error\n");
		g_global.exit_number = 2;
		return (1);
	}
	return (0);
}
