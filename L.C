char	*get_environment_variable(char **env, char *key)
{
	int	i;
	int	j;
	int	key_len;

	i = -1;
	while (env[++i])
	{
		j = 0;
		key_len = ft_strlen(key);
		while (env[i][j] && env[i][j] == key[j] && j < key_len)
			j++;
		if (j == key_len && env[i][j] == '=')
			return (gc_strdup(env[i] + j + 1));
	}
	return (NULL);
}

int	count_array_elements(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return (0);
	while (array[i])
	{
		i++;
	}
	return (i);
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return;
	i = 0;
	while (array[i])
	{
		gc_remove_ptr(array[i]);
		i++;
	}
	gc_remove_ptr(array);
}

int	calculate_length(long number)
{
	int	length;

	length = 0;
	if (number < 0)
	{
		number *= -1;
		length++;
	}
	else if (number == 0)
		length++;
	while (number >= 1)
	{
		number = number / 10;
		length++;
	}
	return (length);
}
