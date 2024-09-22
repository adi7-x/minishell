/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:22:45 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 16:22:07 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*append_char_to_string(char *str, char c)
{
	int		i;
	char	*new;

	i = ft_strlen(str);
	new = gc_malloc(i + 2);
	if (!new)
		return (NULL);
	i = 0;
	while (str && str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i++] = c;
	new[i] = '\0';
	if (str)
		gc_remove_ptr(str);
	return (new);
}

char	*join_strings(char *s1, char *ss2)
{
	int		i;
	int		j;
	char	*result;

	if (!s1 && !ss2)
		return (NULL);
	if (!s1)
	{
		result = gc_strdup(ss2);
		return (result);
	}
	if (!ss2)
		return (gc_strdup(s1));
	i = ft_strlen(s1);
	j = ft_strlen(ss2);
	result = ((char *)gc_malloc(i + j + 1));
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, i);
	ft_memcpy(result + i, ss2, j);
	result[i + j] = '\0';
	return (result);
}

char	*ft_strsrch(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	return (NULL);
}

char	*ft_substr(char *s, int start, int len)
{
	char	*str;
	int		i;

	i = 0;
	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
		return (gc_strdup(""));
	if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	str = (char *)gc_malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*remove_qout(char *str)
{
	t_var	var;
	char	*newstr;

	var.i = 0;
	var.single_quote = 1;
	var.double_quote = 1;
	newstr = NULL;
	while (str[var.i])
	{
		if (str[var.i] == '\'' && var.double_quote == 1)
			var.single_quote *= (-1);
		if (str[var.i] == '\"' && var.single_quote == 1)
			var.double_quote *= (-1);
		if (str[var.i] == '\'' && var.double_quote == 1)
			;
		else if (str[var.i] == '\"' && var.single_quote == 1)
			;
		else
			newstr = append_char_to_string(newstr, str[var.i]);
		var.i++;
	}
	if (newstr == NULL)
		newstr = append_char_to_string(newstr, '\0');
	return (newstr);
}
