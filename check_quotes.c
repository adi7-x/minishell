/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:41:18 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/13 15:41:47 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char *str)
{
	int	i;
	int	sinqot;
	int	dbqot;

	i = 0;
	sinqot = 1;
	dbqot = 1;
	while (str[i] != '\0')
	{
		if (str[i] == '\"' && sinqot == 1)
			dbqot *= (-1);
		if (str[i] == '\'' && dbqot == 1)
			sinqot *= (-1);
		i++;
	}
	if (dbqot == -1 || sinqot == -1)
		return (0);
	return (1);
}
