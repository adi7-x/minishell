/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:52:49 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 15:58:03 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**expand_word_with_variables(char *str, char **envp, int flg)
{
	t_var	var;

	init_var(&var);
	if (!var.new_str)
		return (NULL);
	check_for_variable_expansion(str, &var);
	while (str[var.i])
	{
		var.var = NULL;
		handle_quotes(str[var.i], &var);
		handle_expansion_condition(&var, str, envp, flg);
	}
	return (var.new_str);
}
