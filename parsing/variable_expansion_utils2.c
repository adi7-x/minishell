/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:44:09 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 16:44:54 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*append_ctos(char *str, char c)
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
	if (ft_isalpha(str[var->i + 1]))
	{
		while (str[++var->i] && ft_isalnum(str[var->i]))
			var->var = append_char_to_string(var->var, str[var->i]);
		expand_environment_variable(var, envp, flg);
	}
	else if (isdigit(str[var->i + 1]))
		var->i += 2;
	else if (str[var->i + 1] == '?')
		var->i += handle_exit_status(var);
	else if (str[var->i + 1] == '\0')
		var->new_str[var->count] = append_ctos(var->new_str[var->count],
				str[var->i++]);
	else
	{
		var->new_str[0] = append_char_to_string(var->new_str[0], str[var->i]);
		var->i++;
	}
}

void	handle_expansion_condition(t_var *var, char *str, char **envp, int flg)
{
	if (str[var->i] == '$' && var->single_quote == 1)
		handle_dollar_sign(var, str, envp, flg);
	else if (str[var->i] != '\0')
		append_char_to_new_string(str, var);
	if (!var->new_str[0] && (var->double_quote == -1
			|| var->single_quote == -1))
		var->new_str[0] = append_char_to_string(var->new_str[0], '\0');
}
