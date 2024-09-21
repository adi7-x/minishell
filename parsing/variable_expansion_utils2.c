/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:44:09 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/21 19:01:22 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_var(t_var *var)
{
	var->i = 0;
	var->single_quote = 1;
	var->double_quote = 1;
	var->count = 0;
	var->new_str = ft_calloc(sizeof(char *), 2);
}

void	handle_quotes(char c, t_var *var)
{
	if (c == '\'' && var->double_quote == 1)
		var->single_quote *= -1;
	else if (c == '\"' && var->single_quote == 1)
		var->double_quote *= -1;
}

void	handle_dollar_sign(t_var *var, char *str, char **envp, int flg)
{
	if (isalpha(str[var->i + 1]))
	{
		while (str[++var->i] && isalnum(str[var->i]))
			var->var = append_char_to_string(var->var, str[var->i]);
		expand_environment_variable(var, envp, flg);
	}
	else if (isdigit(str[var->i + 1]))
		var->i += 2;
	else if (str[var->i + 1] == '?')
		var->i += handle_exit_status(var);
	else if (str[var->i + 1] == '\0')
		var->new_str[var->count] = append_char_to_string(var->new_str[var->count],
				str[var->i++]);
}

void	handle_expansion_condition(t_var *var, char *str, char **envp, int flg)
{
	if (str[var->i] == '$' && var->single_quote == 1)
		handle_dollar_sign(var, str, envp, flg);
	else if (str[var->i] != '\0')
		append_char_to_new_string(str, var);
	if (!var->new_str[0] && (var->double_quote == -1 || var->single_quote ==
			-1))
		var->new_str[0] = append_char_to_string(var->new_str[0], '\0');
}
