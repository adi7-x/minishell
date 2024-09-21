/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:09:20 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/21 18:20:41 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_exit_status_in_heredoc(t_var *var)
{
	char	*str;
	char	*s;

	s = ft_itoa(g_global.exit_number);
	str = join_strings(var->str, s);
	if (var->str)
		gc_remove_ptr(var->str);
	gc_remove_ptr(s);
	var->str = str;
	return (2);
}

void	handle_variable_expansion_in_heredoc(t_var *var, char *str, char **envp)
{
	char	*path;
	char	*s;

	s = NULL;
	if (str[var->i] == '$' && isalpha(str[var->i + 1]))
	{
		while (str[++var->i] && isalnum(str[var->i]))
			var->var = append_char_to_string(var->var, str[var->i]);
	}
	else if (str[var->i] == '$' && isdigit(str[var->i + 1]))
		var->i += 2;
	else if (str[var->i] != '\0')
	{
		var->str = append_char_to_string(var->str, str[var->i]);
		var->i += 1;
	}
	if (var->var != NULL)
	{
		path = get_environment_variable(envp, var->var);
		gc_remove_ptr(var->var);
		s = join_strings(var->str, path);
		if (var->str)
			gc_remove_ptr(var->str);
		var->str = s;
	}
}

char	*expending_herd(char *str, char **env)
{
	t_var	var;

	var.i = 0;
	var.str = NULL;
	while (str[var.i])
	{
		var.var = NULL;
		if (str[var.i] == '$' && str[var.i + 1] == '\0')
			var.str = append_char_to_string(var.str, str[var.i++]);
		if (str[var.i] == '$' && str[var.i + 1] == '?')
			var.i += handle_exit_status_in_heredoc(&var);
		if (str[var.i] == '\0')
			break ;
		handle_variable_expansion_in_heredoc(&var, str, env);
	}
	gc_remove_ptr(str);
	return (var.str);
}
