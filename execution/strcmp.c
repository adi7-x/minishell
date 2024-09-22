/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strcmp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 10:30:45 by elcid             #+#    #+#             */
/*   Updated: 2024/09/22 15:58:03 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_comparison_length(char *str, int *len)
{
	char	*end;

	end = strchr(str, '=');
	if (end)
		*len = (int)(end - str);
	else
		*len = strlen(str);
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
	cmp = strncmp(a, b, min_len);
	if (cmp == 0)
	{
		if (a_len != b_len)
			return (a_len - b_len);
		return (strcmp(a, b));
	}
	return (cmp);
}
