/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:07:41 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 16:04:25 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_lexer(t_lexer *lexer)
{
	t_lexer	*current;
	t_lexer	*next;

	current = lexer;
	while (current)
	{
		next = current->next;
		gc_remove_ptr(current->data);
		gc_remove_ptr(current);
		current = next;
	}
}

void	free_parsed_data(t_data *data)
{
	gc_remove_ptr(data);
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		gc_remove_ptr(current->name);
		gc_remove_ptr(current->value);
		gc_remove_ptr(current);
		current = next;
	}
}
