/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:21:22 by elcid             #+#    #+#             */
/*   Updated: 2024/09/22 17:05:44 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest[i] != '\0')
		i++;
	j = 0;
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*strjoin(char *s1, char *s2, char *delimiter)
{
	int		len2;
	int		len1;
	int		len_delim;
	char	*result;

	if (s2 != NULL)
		len2 = ft_strlen(s2);
	else
		len2 = 0;
	len1 = ft_strlen(s1);
	len_delim = ft_strlen(delimiter);
	result = gc_malloc(len1 + len2 + len_delim + 1);
	if (!result)
		return (NULL);
	ft_strcpy(result, s1);
	ft_strcat(result, delimiter);
	if (s2)
		strcat(result, s2);
	return (result);
}

char	*ft_strtok(char *str, char sepa)
{
	static char	*stock = NULL;
	char		*ptr;

	if (str != NULL)
	{
		if (stock != NULL)
			gc_remove_ptr(stock);
		stock = gc_strdup(str);
	}
	if (stock == NULL)
		return (NULL);
	if (*stock == sepa)
		stock++;
	if (*stock == '\0')
		return (NULL);
	ptr = stock;
	while (*stock != '\0' && *stock != sepa)
		stock++;
	if (*stock == sepa)
	{
		*stock = '\0';
		stock++;
	}
	return (ptr);
}
