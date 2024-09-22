/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:07:59 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 16:22:07 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*gc_malloc(size_t size)
{
	void		*ptr;
	t_gc_node	*new_node;

	ptr = malloc(size);
	if (ptr)
	{
		new_node = malloc(sizeof(t_gc_node));
		if (!new_node)
		{
			free(ptr);
			return (NULL);
		}
		new_node->ptr = ptr;
		new_node->is_freed = 0;
		new_node->next = g_global.gc_list;
		g_global.gc_list = new_node;
	}
	return (ptr);
}

void	gc_free_all(void)
{
	t_gc_node	*current;
	t_gc_node	*next;

	current = g_global.gc_list;
	while (current)
	{
		next = current->next;
		if (current->is_freed == 0)
		{
			free(current->ptr);
		}
		free(current);
		current = next;
	}
	g_global.gc_list = NULL;
}

void	gc_remove_ptr(void *ptr)
{
	t_gc_node	*current;

	current = g_global.gc_list;
	while (current)
	{
		if (current->ptr == ptr && current->is_freed == 0)
		{
			current->is_freed = 1;
			free(current->ptr);
			return ;
		}
		current = current->next;
	}
}

char	*gc_strdup(char *s)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(s) + 1;
	new_str = gc_malloc(len);
	if (new_str)
		ft_memcpy(new_str, s, len);
	return (new_str);
}

char	*gc_strndup(char *s, size_t n)
{
	char	*result;

	result = gc_malloc(n + 1);
	if (result)
	{
		ft_strncpy(result, s, n);
		result[n] = '\0';
	}
	return (result);
}
