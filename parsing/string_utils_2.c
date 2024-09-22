/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:39:58 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 18:56:21 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	count_non__words(const char *str)
{
	int	i;
	int	count;

	i = 0;
	if (!str)
		return (0);
	count = 0;
	while (str[i] != '\0')
	{
		if (!is_whitespace_char(str[i]))
		{
			count++;
			while (str[i] != '\0' && !is_whitespace_char(str[i]))
				i++;
		}
		else
			i++;
	}
	return (count);
}

char	**ft_split(char *str)
{
	char	**words;
	int		i;
	int		j;
	int		num_words;

	num_words = 0;
	i = 0;
	words = (char **)gc_malloc((count_non__words(str) + 1) * sizeof(char *));
	if (!words)
		return (NULL);
	while (str != NULL && str[i] != '\0')
	{
		while (is_whitespace_char(str[i]))
			i++;
		if (str[i] == '\0')
			break ;
		j = 0;
		while (!is_whitespace_char(str[i]) && str[i] != '\0' && j++ >= 0)
			i++;
		words[num_words++] = ft_substr(str, i - j, j);
	}
	words[num_words] = NULL;
	return (words);
}

char	**concatenate_strings(char **str, char **str1)
{
	t_var	var;

	var.i = 0;
	var.j = -1;
	var.count = count_string_array(str) + count_string_array(str1);
	var.new_str = ft_calloc(sizeof(char *), var.count + 1);
	if (!var.new_str)
		return (NULL);
	while (str && str[var.i])
		var.new_str[++var.j] = gc_strdup(str[var.i++]);
	if (var.j == -1)
		var.j = 0;
	var.var = join_strings(var.new_str[var.j], str1[0]);
	if (var.new_str[var.j])
		gc_remove_ptr(var.new_str[var.j]);
	var.new_str[var.j] = var.var;
	var.j++;
	var.i = 1;
	while (str1 && str1[var.i])
		var.new_str[var.j++] = gc_strdup(str1[var.i++]);
	return (var.new_str);
}

int	copy_string_array(char **cmd, char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		cmd[i] = gc_strdup(str[i]);
		i++;
	}
	return (i);
}

char	**add_command_string(char **str, t_lexer *lexer, char **envp)
{
	int		cont;
	t_var	var;

	var.j = 0;
	var.i = 0;
	if (str && str[0] && lexer && lexer->data)
	{
		if (ft_strcmp(str[0], "export") == 0)
			var.new_str = expand_word_with_variables(lexer->data, envp, 0);
		else
			var.new_str = expand_word_with_variables(lexer->data, envp, 1);
	}
	else
		var.new_str = expand_word_with_variables(lexer->data, envp, 1);
	cont = count_string_array(str) + count_string_array(var.new_str);
	var.cmd = ft_calloc(sizeof(char *), cont + 1);
	if (var.cmd == NULL)
		return (NULL);
	if (str)
		var.i = copy_string_array(var.cmd, str);
	copy_string_array(&var.cmd[var.i], var.new_str);
	return (var.cmd);
}
