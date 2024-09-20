/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_v1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:15:51 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/20 09:58:55 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_word_expand(char *str, t_var *var)
{
	int	i;

	i = 0;
	var->flag = 0;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '$')
		{
			var->flag = 1;
			return ;
		}
		i++;
	}
}

void	expand_var(t_var *var, char **envp, int flag)
{
	char	*path;
	char	**str1;

	path = ft_getenv(envp, var->var);
	gc_remove_ptr(var->var);
	if (path && var->single_quote == 1 && var->double_quote == 1 && (flag
			|| var->flag))
	{
		str1 = ft_split_it(path);
		var->new_str = ft_catstr(var->new_str, str1);
		var->count = count_str(var->new_str) - 1;
	}
	else
	{
		var->str = ft_strjoinn(var->new_str[var->count], path);
		gc_remove_ptr(var->new_str[var->count]);
		var->new_str[var->count] = var->str;
	}
}

void	add_to_str(char *str, t_var *var)
{
	if (str[var->i] == '\'' && var->double_quote == 1)
		var->i++;
	else if (str[var->i] == '\"' && var->single_quote == 1)
		var->i++;
	else if (str[var->i] == '$' && var->single_quote == 1
		&& var->double_quote == 1 && (str[var->i + 1] == '\"' || str[var->i
				+ 1] == '\''))
		var->i++;
	else
	{
		var->new_str[var->count] = apend_char_str(var->new_str[var->count],
				str[var->i]);
		var->i += 1;
	}
}

void	other_condition(t_var *var, char *str, char **envp, int flg)
{
	if (str[var->i] == '$' && var->single_quote == 1 && isalpha(str[var->i
				+ 1]))
	{
		while (str[++var->i] && isalnum(str[var->i]))
			var->var = apend_char_str(var->var, str[var->i]);
	}
	else if (str[var->i] == '$' && isdigit(str[var->i + 1]))
		var->i += 2;
	else if (str[var->i] != '\0')
		add_to_str(str, var);
	if (var->var != NULL)
	{
		expand_var(var, envp, flg);
	}
	if (!var->new_str[0] && (var->double_quote == -1 || var->single_quote == -1))
		var->new_str[0] = apend_char_str(var->new_str[0], '\0');
}

int	ft_exit_status(t_var *var)
{
	char	*str;
	char	*s;

	s = ft_itoa(g_global.exit_number);
	str = ft_strjoinn(var->new_str[var->count], s);
	if (var->new_str)
		gc_remove_ptr(var->new_str[var->count]);
	gc_remove_ptr(s);
	var->new_str[var->count] = str;
	return (2);
}

char	**ft_expending_word(char *str, char **envp, int flg)
{
	t_var	var;

	var.i = 0;
	var.single_quote = 1;
	var.double_quote = 1;
	var.count = 0;
	var.new_str = ft_calloc(sizeof(char *), 2);
	if (!var.new_str)
		return (NULL);
	check_word_expand(str, &var);
	while (str[var.i])
	{
		var.var = NULL;
		if (str[var.i] == '\'' && var.double_quote == 1)
			var.single_quote *= (-1);
		else if (str[var.i] == '\"' && var.single_quote == 1)
			var.double_quote *= (-1);
		if (str[var.i] == '$' && str[var.i + 1] == '\0')
			var.new_str[var.count] = apend_char_str(var.new_str[var.count],
					str[var.i++]);
		else if (str[var.i] == '$' && str[var.i + 1] == '?'
			&& var.single_quote == 1)
			var.i += ft_exit_status(&var);
		else
			other_condition(&var, str, envp, flg);
	}
	return (var.new_str);
}
