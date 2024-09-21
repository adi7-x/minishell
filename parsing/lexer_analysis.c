/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_analysis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:02:17 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/21 19:46:42 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_token_character(char c, int *single_quote_state, int *double_quote_state)
{
	if (c == '\'' && *double_quote_state != -1)
		*single_quote_state *= -1;
	if (c == '\"' && *single_quote_state != -1)
		*double_quote_state *= -1;
	if (ft_strsrch("()&;<>|\t\v\n ", c) && (*single_quote_state != -1
			&& *double_quote_state != -1))
		return (0);
	else
		return (1);
}

void	ft_lstadd_back(t_lexer **lst, t_lexer *new)
{
	t_lexer	*temp;

	if (!lst || !new)
		return ;
	if (*lst)
	{
		temp = *lst;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new;
		new->prev = temp;
	}
	else
		*lst = new;
}

void	append_token_to_lexer(t_lexer **lexer, char *token_data, int token_type)
{
	t_lexer	*new_token;
	int		token_length;

	token_length = ft_strlen(token_data);
	if (token_type == TOKEN_INREDIR && token_length == 2)
		token_type = TOKEN_HEREDOC;
	else if (token_type == TOKEN_OUTREDIR && token_length == 2)
		token_type = TOKEN_REDIR_APPEND;
	new_token = ft_calloc(sizeof(t_lexer), 1);
	if (new_token == NULL)
		return ;
	new_token->data = token_data;
	new_token->type = token_type;
	ft_lstadd_back(lexer, new_token);
}

void	lexer_analysis(char *input, t_lexer **lexer)
{
	int	i;

	i = 0;
	*lexer = NULL;
	while (input[i])
	{
		if (input[i] == '>')
			i += handle_output_redirection(lexer, &input[i]);
		else if (input[i] == '<')
			i += handle_input_redirection(lexer, &input[i]);
		else if (input[i] == '|')
			i += handle_pipe(lexer, &input[i]);
		else if (is_whitespace_char(input[i]))
			i++;
		else
			i += handle_word(lexer, &input[i]);
	}
	process_syntax_check(lexer);
}
