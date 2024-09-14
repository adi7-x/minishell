#include "minishell.h"

int	ft_exit_statu(t_var *var)
{
	char	*str;
	char	*s;

	s = ft_itoa(g_global.exit_number);
	str = ft_strjoinn(var->str, s);
	if (var->str)
		free(var->str);
	free(s);
	var->str = str;
	return (2);
}

void	other_conditio(t_var *var, char *str, char **envp)
{
	char	*path;
	char	*s;

	s = NULL;
	if (str[var->i] == '$' && isalpha(str[var->i + 1]))
	{
		while (str[++var->i] && isalnum(str[var->i]))
			var->var = apend_char_str(var->var, str[var->i]);
	}
	else if (str[var->i] == '$' && isdigit(str[var->i + 1]))
		var->i += 2;
	else if (str[var->i] != '\0')
	{
		var->str = apend_char_str(var->str, str[var->i]);
		var->i += 1;
	}
	if (var->var != NULL)
	{
		path = ft_getenv(envp, var->var);
		free(var->var);
		s = ft_strjoinn(var->str, path);
		if (var->str)
			free(var->str);
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
			var.str = apend_char_str(var.str, str[var.i++]);
		if (str[var.i] == '$' && str[var.i + 1] == '?')
			var.i += ft_exit_statu(&var);
		if (str[var.i] == '\0')
			break ;
		other_conditio(&var, str, env);
	}
	free(str);
	return (var.str);
}