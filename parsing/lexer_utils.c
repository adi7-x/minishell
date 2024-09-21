/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:16:10 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/21 19:46:59 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_word(t_lexer **lexer, char *len)
{
	int		singl;
	int		doubl;
	int		i;
	char	*str;

	i = 0;
	singl = 1;
	doubl = 1;
	while (len[i] && is_token_character(len[i], &singl, &doubl))
		i++;
	str = gc_malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	append_token_to_lexer(lexer, str, TOKEN_WORD);
	return (i);
}

int	handle_output_redirection(t_lexer **lexer, char *input)
{
	int		i;
	char	*redirection_str;

	i = 0;
	while (input[i] == '>')
		i++;
	redirection_str = gc_malloc(i + 1);
	if (redirection_str == NULL)
		return (0);
	ft_strncpy(redirection_str, input, i);
	append_token_to_lexer(lexer, redirection_str, TOKEN_OUTREDIR);
	return (i);
}

int	handle_input_redirection(t_lexer **lexer, char *len)
{
	int		i;
	char	*str;

	i = 0;
	while (len[i] == '<')
		i++;
	str = gc_malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	append_token_to_lexer(lexer, str, TOKEN_INREDIR);
	return (i);
}

int	handle_pipe(t_lexer **lexer, char *len)
{
	int		i;
	char	*str;

	i = 0;
	while (len[i] == '|')
		i++;
	str = gc_malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	append_token_to_lexer(lexer, str, TOKEN_PIPE);
	return (i);
}
