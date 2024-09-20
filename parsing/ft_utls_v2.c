/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utls_v2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:39:58 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/20 09:50:55 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	cont_w1(const char *str)
{
	int	i;
	int	count;

	i = 0;
	if (!str)
		return (0);
	count = 0;
	while (str[i] != '\0')
	{
		if (!is_whitespace_char(str[i]))
		{
			count++;
			while (str[i] != '\0' && !is_whitespace_char(str[i]))
				i++;
		}
		else if (is_whitespace_char(str[i]))
			i++;
	}
	return (count);
}

char	**ft_split_it(char *str)
{
	char	**p;
	int		i;
	int		j;
	int		n;

	n = 0;
	i = 0;
	p = (char **)gc_malloc((cont_w1(str) + 1) * sizeof(char *));
	if (!p)
		return (NULL);
	while (str != NULL && str[i] != '\0')
	{
		while (is_whitespace_char(str[i]))
			i++;
		if (str[i] == '\0')
			break ;
		j = 0;
		while (!is_whitespace_char(str[i]) && str[i] != '\0' && j++ >= 0)
			i++;
		p[n++] = ft_substr(str, i - j, j);
	}
	p[n] = NULL;
	return (p);
}

char	**ft_catstr(char **str, char **str1)
{
	t_var	var;

	var.i = 0;
	var.j = -1;
	var.count = count_str(str) + count_str(str1);
	var.new_str = ft_calloc(sizeof(char *), var.count + 1);
	if (!var.new_str)
		return (NULL);
	while (str && str[var.i])
		var.new_str[++var.j] = gc_strdup(str[var.i++]);
	if (var.j == -1)
		var.j = 0;
	var.var = ft_strjoinn(var.new_str[var.j], str1[0]);
	if (var.new_str[var.j])
		gc_remove_ptr(var.new_str[var.j]);
	var.new_str[var.j] = var.var;
	var.j++;
	var.i = 1;
	while (str1 && str1[var.i])
		var.new_str[var.j++] = gc_strdup(str1[var.i++]);
	ft_free1(str);
	ft_free1(str1);
	return (var.new_str);
}

int	ft_copy(char **cmd, char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		cmd[i] = gc_strdup(str[i]);
		i++;
	}
	return (i);
}

char	**ft_addstring(char **str, t_lexer *lexer, char **envp)
{
	int		cont;
	t_var	var;

	var.j = 0;
	var.i = 0;
	if (str && str[0] && lexer && lexer->data)
	{
		if (strcmp(str[0], "export") == 0)
			var.new_str = ft_expending_word(lexer->data, envp, 0);
		else
			var.new_str = ft_expending_word(lexer->data, envp, 1);
	}
	else
		var.new_str = ft_expending_word(lexer->data, envp, 1);
	cont = count_str(str) + count_str(var.new_str);
	var.cmd = ft_calloc(sizeof(char *), cont + 1);
	if (var.cmd == NULL)
		return (NULL);
	if (str)
		var.i = ft_copy(var.cmd, str);
	ft_copy(&var.cmd[var.i], var.new_str);
	ft_free1(str);
	if (str)
		ft_free1(str);
	return (var.cmd);
}

char	*ft_itoa(int nb)
{
	int		i;
	char	*p;
	long	b;

	b = nb;
	i = ft_len(b);
	p = gc_malloc(i + 1);
	if (p == NULL)
		return (0);
	p[i] = '\0';
	if (b < 0)
	{
		p[0] = '-';
		b *= (-1);
	}
	if (b == 0)
		p[i - 1] = 48;
	while (b > 0)
	{
		p[i - 1] = (b % 10) + '0';
		i--;
		b /= 10;
	}
	return (p);
}
