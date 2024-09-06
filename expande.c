#include "minishell.h"

char	*ft_strjoinn(char *s1, char *ss2)
{
	int		i;
	int		j;
	char	*result;

	if (!s1 && !ss2)
		return (NULL);
	if (!s1)
	{
		result = strdup(ss2);
		return (result);
	}
	if (!ss2)
		return ((char *)s1);
	i = strlen(s1);
	j = strlen(ss2);
	result = ((char *)malloc(i + j + 1));
	if (!result)
		return (NULL);
	memcpy(result, s1, i);
	memcpy(result + i, ss2, j);
	result[i + j] = '\0';
	return (result);
}

static int	ft_len(long nb)
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

char	*ft_itoa(int nb)
{
	int		i;
	char	*p;
	long	b;

	b = nb;
	i = ft_len(b);
	p = malloc(i + 1);
	if (p == NULL)
		return (0);
	p[i] = '\0';
	if (b < 0)
	{
		p[0] = '-';
		b *= (-1);
	}
	if (b == 0)
		p[i - 1] = 48;
	while (b > 0)
	{
		p[i - 1] = (b % 10) + '0';
		i--;
		b /= 10;
	}
	return (p);
}

char	*apend_char_str(char *str, char c)
{
	int		i;
	char	*new;

	i = 0;
	if (str == NULL)
	{
		if (!(new = malloc(2)))
			return (NULL);
		new[0] = c;
		new[1] = '\0';
		return (new);
	}
	i = strlen(str);
	if (!(new = malloc(i + 2)))
		return (NULL);
	i = 0;
	while (str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i++] = c;
	new[i] = '\0';
	free(str);
	return (new);
}

char	*ft_getenv(t_env *env, char *str)
{
	int	i;

	if (!str)
		return (NULL);
	while (env)
	{
		i = 0;
		while (str[i] && str[i] == env->name[i])
			i++;
		if (str[i] == '\0' && env->name[i] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
/*
char	*expend_it(char *var, char *newstr, t_env *envp)
{
	int		j;
	char	*path;

	j = -1;
	path = ft_getenv(envp, var);
	free(var);
	while (path && path[++j])
		newstr = apend_char_str(newstr, path[j]);
	return (newstr);
}

*/

char	*expend_it(char *var, char *newstr, t_env *envp)
{
	int		j;
	char	*path;

	if (!var) // Check if var is NULL to avoid unnecessary operations
		return (newstr);
	j = -1;
	path = ft_getenv(envp, var);
	free(var); // Ensure var is only freed here and not elsewhere
	if (!path)
		return (newstr); // Return if no matching environment variable found
	while (path[++j])
		newstr = apend_char_str(newstr, path[j]);
	return (newstr);
}
char	*add_to_str(char *str, t_var var, int *i)
{
	if (str[*i] == '$' && var.sinqot == 1 && var.dobqot == 1 && (str[*i
			+ 1] == '\"' || str[*i + 1] == '\''))
		*i += 1;
	else
	{
		var.newstr = apend_char_str(var.newstr, str[*i]);
		*i += 1;
	}
	return (var.newstr);
}
/*
char	*ft_expending_word(char *str, t_env *envp)
{
	int		i;
	t_var	var;
	char	*temp;

	i = 0;
	var.sinqot = 1;
	var.dobqot = 1;
	var.newstr = NULL;
	while (str[i])
	{
		var.var = NULL;
		if (str[i] == '\'' && var.dobqot == 1)
			var.sinqot *= (-1);
		else if (str[i] == '\"' && var.sinqot == 1)
			var.dobqot *= (-1);
		if (str[i] == '$' && str[i + 1] == '\0')
			var.newstr = apend_char_str(var.newstr, str[i++]);
		if (str[i] == '$' && str[i + 1] == '?')
		{
			temp = ft_itoa(envp->exit_status);
			var.newstr = ft_strjoinn(var.newstr, temp);
			free(temp);
			i += 2;
		}
		if (str[i] == '\0')
			break ;
		else if (str[i] == '$' && var.sinqot == 1 && isalpha(str[i + 1]))
		{
			i++; // Move past the '$'
			while (str[i] && isalnum(str[i]))
				var.var = apend_char_str(var.var, str[i++]);
			var.newstr = expend_it(var.var, var.newstr, envp);
			// var.var is freed in expend_it, so don't free it here
		}
		else if (str[i] == '$' && isdigit(str[i + 1]))
			i += 2;
		else if (str[i] != '\0')
			var.newstr = add_to_str(str, var, &i);
	}
	free(str);
	return (var.newstr);
}
*/

// this is the good version likant 9bal mayji marakchi
/*
char	*ft_expending_word(char *str, t_env *envp)
{
	int		i;
	t_var	var;
	char	*temp;

	i = 0;
	var.sinqot = 1;
	var.dobqot = 1;
	var.newstr = NULL;
	while (str[i])
	{
		var.var = NULL; // Always start with var being NULL
		if (str[i] == '\'' && var.dobqot == 1)
			var.sinqot *= (-1);
		else if (str[i] == '\"' && var.sinqot == 1)
			var.dobqot *= (-1);
		if (str[i] == '$' && str[i + 1] == '\0')
			var.newstr = apend_char_str(var.newstr, str[i++]);
		else if (str[i] == '$' && str[i + 1] == '?')
		{
			temp = ft_itoa(g_exit_status);
			var.newstr = ft_strjoinn(var.newstr, temp);
			free(temp);
			i += 2;
		}
		else if (str[i] == '$' && var.sinqot == 1 && isalpha(str[i + 1]))
		{
			i++;
			while (str[i] && isalnum(str[i]))
				var.var = apend_char_str(var.var, str[i++]);
			var.newstr = expend_it(var.var, var.newstr, envp);
		}
		else if (str[i] == '$' && isdigit(str[i + 1]))
			i += 2;
		else if (str[i] != '\0')
			var.newstr = add_to_str(str, var, &i);
	}
	free(str);
	return (var.newstr);
}
*/

char	*ft_expending_word(char *str, t_env *envp)
{
	int		i;
	t_var	var;
	char	*temp;

	i = 0;
	var.sinqot = 0;
	var.dobqot = 0;
	var.newstr = NULL;
	while (str[i])
	{
		var.var = NULL;
		if (str[i] == '\'' && !var.dobqot)
		{
			var.sinqot = !var.sinqot;
			i++;
			continue ;
		}
		else if (str[i] == '\"' && !var.sinqot)
		{
			var.dobqot = !var.dobqot;
			i++;
			continue ;
		}
		if (str[i] == '$' && str[i + 1] == '\0' && !var.sinqot)
			var.newstr = apend_char_str(var.newstr, str[i++]);
		else if (str[i] == '$' && str[i + 1] == '?' && !var.sinqot)
		{
			temp = ft_itoa(g_exit_status);
			var.newstr = ft_strjoinn(var.newstr, temp);
			free(temp);
			i += 2;
		}
		else if (str[i] == '$' && !var.sinqot && isalpha(str[i + 1]))
		{
			i++;
			while (str[i] && isalnum(str[i]))
				var.var = apend_char_str(var.var, str[i++]);
			var.newstr = expend_it(var.var, var.newstr, envp);
		}
		else if (str[i] == '$' && isdigit(str[i + 1]) && !var.sinqot)
			i += 2;
		else if (str[i] != '\0')
			var.newstr = apend_char_str(var.newstr, str[i++]);
	}
	free(str);
	return (var.newstr);
}
/*
int	expander(t_lexer *lexer_output, t_env *envp)
{
	t_lexer	*current;
	char	*expanded_word;

	current = lexer_output;
	while (current)
	{
		// Expand the current token's data
		expanded_word = ft_expending_word(current->data, envp);
		if (!expanded_word)
			return (0); // Expansion failed, return an error
		// Free the old data and assign the new expanded data
		// free(current->data);
		current->data = expanded_word;
		// Move to the next token
		current = current->next;
	}
	return (1); // Expansion successful
}

*/

int	expander(t_lexer *lexer_output, t_env *envp)
{
	t_lexer *current;
	char *expanded_word;

	current = lexer_output;
	while (current)
	{
		expanded_word = ft_expending_word(current->data, envp);
		if (!expanded_word)
			return (0); // Expansion failed, return an error
		// free(current->data);           // Free the old data
		current->data = expanded_word; // Assign the new expanded data
		current = current->next;
	}
	return (1); // Expansion successful
}