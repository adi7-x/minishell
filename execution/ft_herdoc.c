/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_herdoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elcid <elcid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:07:26 by adbourji          #+#    #+#             */
/*   Updated: 2024/09/18 11:07:32 by elcid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_herdoc_and_exit(int exit_s, char *str)
{
	if (str)
		printf("%s\n", str);
	clear_history();
	cleanup();
	exit(exit_s);
}

int	cont_herd(t_data *data)
{
	t_file	*newfile;
	int		i;

	i = 0;
	while (data)
	{
		newfile = data->file;
		while (newfile)
		{
			if (newfile->heredoc == 1)
				i++;
			newfile = newfile->next;
		}
		data = data->next;
	}
	return (i);
}

int	wait_herdoc(int pid, t_file *newfile)
{
	int	status;

	waitpid(pid, &status, 0);
	if ((status >> 8) == 130)
	{
		write(1, "\n", 2);
		g_global.exit_number = 130;
		close(newfile->fd[1]);
		return (1);
	}
	close(newfile->fd[1]);
	return (0);
}

void	open_herdoc(t_file *newfile, char **env)
{
	char	*line;

	signal(SIGINT, handle_heredoc_signal);
	while (1)
	{
		line = readline(">");
		if (line == NULL)
			break ;
		if (strcmp(line, newfile->file_name) == 0)
		{
			gc_remove_ptr(line);
			free_herdoc_and_exit(0, NULL);
		}
		line = apend_char_str(line, '\n');
		if (!newfile->expand)
			line = expending_herd(line, env);
		write(newfile->fd[1], line, strlen(line));
		gc_remove_ptr(line);
	}
	if (line == NULL)
	{
		printf("here-document delimited by end-of-file (wanted `%s')\n",
			newfile->file_name);
		free_herdoc_and_exit(0, NULL);
	}
}

int	ft_herdoc(t_data *data, char **env)
{
	t_file	*newfile;
	int		pid;

	signal(SIGINT, SIG_IGN);
	if (cont_herd(data) > 16)
		free_herdoc_and_exit(2, "maximum here-document count exceeded");
	while (data)
	{
		newfile = data->file;
		while (newfile)
		{
			if (newfile->heredoc == 1)
			{
				pipe(newfile->fd);
				pid = fork();
				if (pid == 0)
					open_herdoc(newfile, env);
				if (wait_herdoc(pid, newfile))
					return (1);
			}
			newfile = newfile->next;
		}
		data = data->next;
	}
	return (0);
}
