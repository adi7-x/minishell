/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_analysis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:02:17 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/19 12:36:48 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_token_character(char c, int *single_quote_state, int *double_quote_state)
{
    if (c == '\'' && *double_quote_state != -1)
        *single_quote_state *= -1;
    if (c == '\"' && *single_quote_state != -1)
        *double_quote_state *= -1;
    if (ft_strsrch("()&;<>|\t\v\n ", c) && (*single_quote_state != -1 && *double_quote_state != -1))
        return (0);
    else
        return (1);
}

char	*ft_strncpy(char *dest, char *src, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	is_whitespace_char(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;

	if (count != 0 && size > SIZE_MAX / count)
		return (0);
	p = gc_malloc(count * size);
	if (p == NULL)
		return (NULL);
	memset(p, 0, count * size);
	return (p);
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

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void append_token_to_lexer(t_lexer **lexer, char *token_data, int token_type)
{
    t_lexer *new_token;
    int token_length;

    token_length = ft_strlen(token_data);
    if (token_type == TOKEN_INREDIR && token_length == 2)
        token_type = TOKEN_HEREDOC;
    else if (token_type == TOKEN_OUTREDIR && token_length == 2)
        token_type = TOKEN_REDIR_APPEND;
    new_token = ft_calloc(sizeof(t_lexer), 1);
    if (new_token == NULL)
        return;
    new_token->data = token_data;
    new_token->type = token_type;
    ft_lstadd_back(lexer, new_token);
}



int handle_output_redirection(t_lexer **lexer, char *input)
{
    int i;
    char *redirection_str;

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
int handle_semicolon(t_lexer **lexer, char *input)
{
	(void)input;
    char *semicolon_str = gc_malloc(2);
    if (semicolon_str == NULL)
        return (0);
    semicolon_str[0] = ';';
    semicolon_str[1] = '\0';
    append_token_to_lexer(lexer, semicolon_str, TOKEN_SEMICOLON);
    return (1);
}

void lexer_analysis(char *input, t_lexer **lexer)
{
    int i;

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
		else if (input[i] == ';')
            i += handle_semicolon(lexer, &input[i]);
        else if (is_whitespace_char(input[i]))
            i++;
        else
            i += handle_word(lexer, &input[i]);
    }
    check_syntax_errors(lexer);
}