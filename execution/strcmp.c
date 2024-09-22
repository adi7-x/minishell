/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strcmp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 17:04:48 by elcid             #+#    #+#             */
/*   Updated: 2024/09/22 17:04:49 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	get_comparison_length(char *str, int *len)
{
	char	*end;

	end = ft_strchr(str, '=');
	if (end)
		*len = (int)(end - str);
	else
		*len = ft_strlen(str);
	return (*len);
}

int	custom_strcmp(char *a, char *b)
{
	int	a_len;
	int	b_len;
	int	min_len;
	int	cmp;

	get_comparison_length(a, &a_len);
	get_comparison_length(b, &b_len);
	if (a_len < b_len)
		min_len = a_len;
	else
		min_len = b_len;
	cmp = ft_strncmp(a, b, min_len);
	if (cmp == 0)
	{
		if (a_len != b_len)
			return (a_len - b_len);
		return (ft_strcmp(a, b));
	}
	return (cmp);
}
