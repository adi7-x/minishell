/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_utils_v1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:22:15 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/20 16:47:56 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
