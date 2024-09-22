/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handling_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:22:15 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/22 15:58:03 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_file_to_list(t_file **file, t_file *new)
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

void	handle_ambiguous_redirection(char **namfile, t_file **file,
		t_file *newfile, char *s)
{
	free_string_array(namfile);
	newfile->file_name = s;
	newfile->ambiguous = 1;
	add_file_to_list(file, newfile);
}

void	set_file_type(int type, t_file *newfile)
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

void	handle_file_redirection(t_lexer *lexer, int type, t_file **file,
		char **envp)
{
	t_file	*newfile;
	char	**namefile;

	newfile = ft_calloc(sizeof(t_file), 1);
	if (newfile == NULL)
		return ;
	if (ft_strsrch(lexer->data, '\'') || ft_strsrch(lexer->data, '\"'))
		newfile->expand = 1;
	if (type != TOKEN_HEREDOC)
		namefile = expand_word_with_variables(lexer->data, envp, 1);
	else
	{
		namefile = ft_calloc(sizeof(char *), 2);
		namefile[0] = remove_qout(lexer->data);
	}
	if (!namefile[0] || count_string_array(namefile) > 1)
	{
		handle_ambiguous_redirection(namefile, file, newfile,
			gc_strdup(lexer->data));
		return ;
	}
	set_file_type(type, newfile);
	newfile->file_name = namefile[0];
	gc_remove_ptr(namefile);
	add_file_to_list(file, newfile);
}
