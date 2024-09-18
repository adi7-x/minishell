/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_end_expand.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:07:13 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/18 17:35:06 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*apend_char_str(char *str, char c)
{
	int		i;
	char	*new;

	i = ft_strlen(str);
	new = gc_malloc(i + 2);
	if (!new)
		return (NULL);
	i = 0;
	while (str && str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i++] = c;
	new[i] = '\0';
	if (str)
		gc_remove_ptr(str);
	return (new);
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

char	*remove_qout(char *str)
{
	t_var	var;
	char	*newstr;

	var.i = 0;
	var.single_quote = 1;
	var.double_quote = 1;
	newstr = NULL;
	while (str[var.i])
	{
		if (str[var.i] == '\'' && var.double_quote == 1)
			var.single_quote *= (-1);
		if (str[var.i] == '\"' && var.single_quote == 1)
			var.double_quote *= (-1);
		if (str[var.i] == '\'' && var.double_quote == 1)
			;
		else if (str[var.i] == '\"' && var.single_quote == 1)
			;
		else
			newstr = apend_char_str(newstr, str[var.i]);
		var.i++;
	}
	if (newstr == NULL)
		newstr = apend_char_str(newstr, '\0');
	return (newstr);
}

char	*ft_strsrch(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	return (NULL);
}

int	ft_len(long nb)
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
	p = gc_malloc(i + 1);
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

static int	cont_w1(const char *str)
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
		else if (is_whitespace_char(str[i]))
			i++;
	}
	return (count);
}
char	*ft_substr(char *s, int start, int len)
{
	char	*str;
	int		i;

	i = 0;
	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
		return (gc_strdup(""));
	if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	str = (char *)gc_malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	cont_w(const char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	if (!str)
		return (0);
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c && str[i] != '\t')
		{
			count++;
			while (str[i] != '\0' && str[i] != c && str[i] != '\t')
				i++;
		}
		else if (str[i] == c || str[i] == '\t')
			i++;
	}
	return (count);
}

char	**ft_split(char *s, char c)
{
	char	**p;
	int		i;
	int		j;
	int		n;

	n = 0;
	i = 0;
	p = (char **)gc_malloc((cont_w(s, c) + 1) * sizeof(char *));
	if (!p)
		return (NULL);
	while (s != NULL && s[i] != '\0')
	{
		while (s[i] == c || s[i] == '\t')
			i++;
		if (s[i] == '\0')
			break ;
		j = 0;
		while (s[i] != c && s[i] != '\t' && s[i] != '\0' && j++ >= 0)
			i++;
		p[n++] = ft_substr(s, i - j, j);
	}
	p[n] = NULL;
	return (p);
}

void	ft_lstadd_back1(t_file **file, t_file *new)
{
	t_file	*tmp;

	if (!file || !new)
		return ;
	if (*file)
	{
		tmp = *file;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*file = new;
}

void	ft_ambiguous(char **namfile, t_file **file, t_file *newfile, char *s)
{
	ft_free1(namfile);
	newfile->file_name = s;
	newfile->ambiguous = 1;
	ft_lstadd_back1(file, newfile);
}

void	type_file(int type, t_file *newfile)
{
	if (type == TOKEN_INREDIR)
		newfile->infile = 1;
	else if (type == TOKEN_OUTREDIR)
		newfile->outfile = 1;
	else if (type == TOKEN_HEREDOC)
		newfile->heredoc = 1;
	else if (type == TOKEN_REDIR_APPEND)
		newfile->append = 1;
}

void	ft_free1(char **str)
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
int	count_str(char **str)
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

char	*ft_getenv(char **env, char *str)
{
	int	i;
	int	j;
	int	len;

	i = -1;
	while (env[++i])
	{
		j = 0;
		len = strlen(str);
		while (env[i][j] && env[i][j] == str[j] && j < len)
			j++;
		if (j == len && env[i][j] == '=')
			return (gc_strdup(env[i] + j + 1));
	}
	return (NULL);
}

char	*ft_strjoinn(char *s1, char *ss2)
{
	int		i;
	int		j;
	char	*result;

	if (!s1 && !ss2)
		return (NULL);
	if (!s1)
	{
		result = gc_strdup(ss2);
		return (result);
	}
	if (!ss2)
		return (gc_strdup(s1));
	i = strlen(s1);
	j = strlen(ss2);
	result = ((char *)gc_malloc(i + j + 1));
	if (!result)
		return (NULL);
	memcpy(result, s1, i);
	memcpy(result + i, ss2, j);
	result[i + j] = '\0';
	return (result);
}

char	**ft_catstr(char **str, char **str1)
{
	t_var	var;

	var.i = 0;
	var.j = -1;
	var.count = count_str(str) + count_str(str1);
	var.new_str = ft_calloc(sizeof(char *), var.count + 1);
	if (!var.new_str)
		return (NULL);
	while (str && str[var.i])
		var.new_str[++var.j] = gc_strdup(str[var.i++]);
	if (var.j == -1)
		var.j = 0;
	var.var = ft_strjoinn(var.new_str[var.j], str1[0]);
	if (var.new_str[var.j])
		gc_remove_ptr(var.new_str[var.j]);
	var.new_str[var.j] = var.var;
	var.j++;
	var.i = 1;
	while (str1 && str1[var.i])
		var.new_str[var.j++] = gc_strdup(str1[var.i++]);
	ft_free1(str);
	ft_free1(str1);
	return (var.new_str);
}

char	**ft_split_it(char *str)
{
	char	**p;
	int		i;
	int		j;
	int		n;

	n = 0;
	i = 0;
	p = (char **)gc_malloc((cont_w1(str) + 1) * sizeof(char *));
	if (!p)
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
		p[n++] = ft_substr(str, i - j, j);
	}
	p[n] = NULL;
	return (p);
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
	if (!var->new_str[0] && (var->double_quote == -1 || var->single_quote ==
			-1))
		var->new_str[0] = apend_char_str(var->new_str[0], '\0');
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

int	ft_copy(char **cmd, char **str)
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

char	**ft_addstring(char **str, t_lexer *lexer, char **envp)
{
	int		cont;
	t_var	var;

	var.j = 0;
	var.i = 0;
	if (str && str[0] && lexer && lexer->data)
	{
		if (strcmp(str[0], "export") == 0)
			var.new_str = ft_expending_word(lexer->data, envp, 0);
		else
			var.new_str = ft_expending_word(lexer->data, envp, 1);
	}
	else
		var.new_str = ft_expending_word(lexer->data, envp, 1);
	cont = count_str(str) + count_str(var.new_str);
	var.cmd = ft_calloc(sizeof(char *), cont + 1);
	if (var.cmd == NULL)
		return (NULL);
	if (str)
		var.i = ft_copy(var.cmd, str);
	ft_copy(&var.cmd[var.i], var.new_str);
	ft_free1(str);
	if (str)
		ft_free1(str);
	return (var.cmd);
}

void	append_to_file(t_lexer *lexer, int type, t_file **file, char **envp)
{
	t_file	*newfile;
	char	**namefile;

	newfile = ft_calloc(sizeof(t_file), 1);
	if (newfile == NULL)
		return ;
	if (ft_strsrch(lexer->data, '\'') || ft_strsrch(lexer->data, '\"'))
		newfile->expand = 1;
	if (type != TOKEN_HEREDOC)
		namefile = ft_expending_word(lexer->data, envp, 1);
	else
	{
		namefile = ft_calloc(sizeof(char *), 2);
		namefile[0] = remove_qout(lexer->data);
	}
	if (!namefile[0] || count_str(namefile) > 1)
	{
		ft_ambiguous(namefile, file, newfile, gc_strdup(lexer->data));
		return ;
	}
	type_file(type, newfile);
	newfile->file_name = namefile[0];
	gc_remove_ptr(namefile);
	ft_lstadd_back1(file, newfile);
}

void	append_to_data(t_data **data, t_file **file, char ***cmd)
{
	t_data	*newdata;
	t_data	*tmp;

	newdata = ft_calloc(sizeof(t_data), 1);
	if (newdata == NULL)
		return ;
	newdata->cmd = *cmd;
	newdata->file = *file;
	*file = NULL;
	tmp = *data;
	if (*data)
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = newdata;
	}
	else
		*data = newdata;
	*cmd = NULL;
}

t_data	*ft_parsing(t_lexer *lexer, char **envp)
{
	t_data	*data;
	t_var	var;

	var.lexer = lexer;
	var.cmd = NULL;
	data = NULL;
	if (lexer == NULL)
		return (NULL);
	var.file = NULL;
	while (lexer)
	{
		if (lexer->type == TOKEN_WORD && lexer->prev == NULL)
			var.cmd = ft_addstring(var.cmd, lexer, envp);
		else if (lexer->type == TOKEN_WORD && lexer->prev->type != TOKEN_WORD
			&& lexer->prev->type != TOKEN_PIPE)
			append_to_file(lexer, lexer->prev->type, &var.file, envp);
		else if (lexer->type == TOKEN_PIPE)
			append_to_data(&data, &var.file, &var.cmd);
		else if (lexer->type == TOKEN_WORD)
			var.cmd = ft_addstring(var.cmd, lexer, envp);
		lexer = lexer->next;
	}
	append_to_data(&data, &var.file, &var.cmd);
	free_lexer(var.lexer);
	return (data);
}

t_env	*convert_env_to_list(char **env)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;
	char	*equals_sign;
	int		name_len;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		new_node = gc_malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		equals_sign = strchr(env[i], '=');
		if (equals_sign)
		{
			name_len = equals_sign - env[i];
			new_node->name = strndup(env[i], name_len);
			new_node->value = gc_strdup(equals_sign + 1);
			new_node->var = gc_strdup(env[i]);
		}
		else
		{
			new_node->name = gc_strdup(env[i]);
			new_node->value = gc_strdup("");
			new_node->var = gc_strdup(env[i]);
		}
		new_node->exit_status = 0;
		new_node->next = env_list;
		env_list = new_node;
		i++;
	}
	return (env_list);
}

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		gc_remove_ptr(current->name);
		gc_remove_ptr(current->value);
		gc_remove_ptr(current->var);
		gc_remove_ptr(current);
		current = next;
	}
}

t_data	*process_input(char *input, t_shell *shell)
{
	t_lexer *lexer_output;
	t_data *data;

	data = NULL;
	lexer_output = NULL;
	lexer_analysis(input, &lexer_output);

	if (!lexer_output)
		return (NULL);

	data = ft_parsing(lexer_output, shell->env);

	if (!data)
	{
		free_lexer(lexer_output);
		return (NULL);
	}

	return (data);
}