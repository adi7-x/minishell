/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:51:47 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/21 19:02:10 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_for_variable_expansion(char *str, t_var *var)
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

void	expand_environment_variable(t_var *var, char **envp, int flag)
{
	char	*path;
	char	**str1;

	path = get_environment_variable(envp, var->var);
	gc_remove_ptr(var->var);
	if (path && var->single_quote == 1 && var->double_quote == 1 && (flag
			|| var->flag))
	{
		str1 = ft_split(path);
		var->new_str = concatenate_strings(var->new_str, str1);
		var->count = count_string_array(var->new_str) - 1;
	}
	else
	{
		var->str = join_strings(var->new_str[var->count], path);
		gc_remove_ptr(var->new_str[var->count]);
		var->new_str[var->count] = var->str;
	}
}

void	append_char_to_new_string(char *str, t_var *var)
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
		var->new_str[var->count] = append_char_to_string(var->new_str[var->count],
				str[var->i]);
		var->i += 1;
	}
}

int	handle_exit_status(t_var *var)
{
	char	*str;
	char	*s;

	s = ft_itoa(g_global.exit_number);
	str = join_strings(var->new_str[var->count], s);
	if (var->new_str)
		gc_remove_ptr(var->new_str[var->count]);
	gc_remove_ptr(s);
	var->new_str[var->count] = str;
	return (2);
}

char	*ft_itoa(int nb)
{
	int		count;
	char	*str;
	long	num;

	num = nb;
	count = calculate_length(num);
	str = gc_malloc(count + 1);
	if (str == NULL)
		return (0);
	str[count] = '\0';
	if (num < 0)
	{
		str[0] = '-';
		num *= (-1);
	}
	if (num == 0)
		str[count - 1] = 48;
	while (num > 0)
	{
		str[count - 1] = (num % 10) + '0';
		count--;
		num /= 10;
	}
	return (str);
}
