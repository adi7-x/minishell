/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_and_array_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:21:41 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/21 18:23:49 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_environment_variable(char **env, char *str)
{
	int	i;
	int	j;
	int	len;

	i = -1;
	while (env[++i])
	{
		j = 0;
		len = ft_strlen(str);
		while (env[i][j] && env[i][j] == str[j] && j < len)
			j++;
		if (j == len && env[i][j] == '=')
			return (gc_strdup(env[i] + j + 1));
	}
	return (NULL);
}

int	count_string_array(char **str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i])
	{
		i++;
	}
	return (i);
}

void	free_string_array(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		gc_remove_ptr(str[i]);
		i++;
	}
	gc_remove_ptr(str);
}

int	calculate_length(long nb)
{
	int	i;

	i = 0;
	if (nb < 0)
	{
		nb *= -1;
		i++;
	}
	else if (nb == 0)
		i++;
	while (nb >= 1)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}
