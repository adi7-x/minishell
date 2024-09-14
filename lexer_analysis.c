/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_analysis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:02:17 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/13 16:08:05 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_token(char c, int *singl, int *doubl)
{
	if (c == '\'' && *doubl != -1)
		*singl *= -1;
	if (c == '\"' && *singl != -1)
		*doubl *= -1;
	if (ft_strsrch("()&;<>|\t\v\n ", c) && (*singl != -1 && *doubl != -1))
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

int	is_whitespace(char c)
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
	p = malloc(count * size);
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

void	apend_in_struct(t_lexer **lexer, char *str, int type)
{
	t_lexer	*new_lexer;
	int		len;

	len = ft_strlen(str);
	if (type == TOKEN_INREDIR && len == 2)
		type = TOKEN_HEREDOC;
	else if (type == TOKEN_OUTREDIR && len == 2)
		type = TOKEN_REDIR_APPEND;
	new_lexer = ft_calloc(sizeof(t_lexer), 1);
	if (new_lexer == NULL)
		return ;
	new_lexer->data = str;
	new_lexer->type = type;
	ft_lstadd_back(lexer, new_lexer);
}

#include "minishell.h"

int	ft_outred(t_lexer **lexer, char *len)
{
	int		i;
	char	*str;

	i = 0;
	while (len[i] == '>')
		i++;
	str = malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	apend_in_struct(lexer, str, TOKEN_OUTREDIR);
	return (i);
}

int	ft_inreder(t_lexer **lexer, char *len)
{
	int		i;
	char	*str;

	i = 0;
	while (len[i] == '<')
		i++;
	str = malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	apend_in_struct(lexer, str, TOKEN_INREDIR);
	return (i);
}

int	ft_pipe(t_lexer **lexer, char *len)
{
	int		i;
	char	*str;

	i = 0;
	while (len[i] == '|')
		i++;
	str = malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	apend_in_struct(lexer, str, TOKEN_PIPE);
	return (i);
}

int	ft_word(t_lexer **lexer, char *len)
{
	int		singl;
	int		doubl;
	int		i;
	char	*str;

	i = 0;
	singl = 1;
	doubl = 1;
	while (len[i] && ft_token(len[i], &singl, &doubl))
		i++;
	str = malloc(i + 1);
	if (str == NULL)
		return (0);
	ft_strncpy(str, len, i);
	apend_in_struct(lexer, str, TOKEN_WORD);
	return (i);
}

void	lexer_analysis(char *len, t_lexer **lexer)
{
	int i;

	i = 0;
	*lexer = NULL;
	while (len[i])
	{
		if (len[i] == '>')
			i += ft_outred(lexer, &len[i]);
		else if (len[i] == '<')
			i += ft_inreder(lexer, &len[i]);
		else if (len[i] == '|')
			i += ft_pipe(lexer, &len[i]);
		else if (is_whitespace(len[i]))
			i++;
		else
			i += ft_word(lexer, &len[i]);
	}
	check_syntax_errors(lexer);
}