/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:26:35 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 15:58:03 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	validate_quotes(char *input)
{
	int	i;
	int	single_quote_state;
	int	double_quote_state;

	i = 0;
	single_quote_state = 1;
	double_quote_state = 1;
	while (input[i] != '\0')
	{
		if (input[i] == '\"' && single_quote_state == 1)
			double_quote_state *= (-1);
		if (input[i] == '\'' && double_quote_state == 1)
			single_quote_state *= (-1);
		i++;
	}
	if (double_quote_state == -1 || single_quote_state == -1)
		return (0);
	return (1);
}
