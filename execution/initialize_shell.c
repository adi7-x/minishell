/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_shell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbourji <adbourji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:51:24 by elcid             #+#    #+#             */
/*   Updated: 2024/09/21 18:20:37 by adbourji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**create_default_env(char *cwd)
{
	char	**new_env;

	new_env = gc_malloc(sizeof(char *) * 5);
	if (!new_env)
		return (NULL);
	new_env[0] = strjoin("PWD=", cwd, "");
	new_env[1] = gc_strdup("SHLVL=1");
	new_env[2] = gc_strdup("_=/usr/bin/env");
	new_env[3] = gc_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:"
			"/usr/bin:/sbin:/bin");
	new_env[4] = NULL;
	return (new_env);
}

char	**copy_env(char **envp, t_shell *shell)
{
	char	**new_env;
	char	cwd[MAX_PATH];
	int		count;
	int		i;

	if (!envp || !*envp)
	{
		getcwd(cwd, sizeof(cwd));
		new_env = create_default_env(cwd);
		shell->ignore_path = 1;
		return (new_env);
	}
	count = 0;
	while (envp[count])
		count++;
	new_env = gc_malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = -1;
	while (++i < count)
		new_env[i] = gc_strdup(envp[i]);
	new_env[count] = NULL;
	shell->ignore_path = 0;
	return (new_env);
}

int	get_num_len(int nbr)
{
	int	len;

	len = 1;
	while (nbr != 0)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

char	*int_to_str(int nbr)
{
	char	*str;
	int		len;

	if (nbr >= 1000)
	{
		printf("bash: warning: shell level (%d) too high, resetting to 1\n",
			nbr);
		return (gc_strdup("1"));
	}
	len = get_num_len(nbr);
	str = gc_malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = nbr % 10 + '0';
		nbr /= 10;
	}
	return (str);
}

void	initialize_shell(t_shell *shell, char **envp)
{
	char	*shlvl;
	int		level;
	char	*new_shlvl;

	shell->env = copy_env(envp, shell);
	g_global.exit_number = 0;
	if (getcwd(shell->cwd, sizeof(shell->cwd)) == NULL)
	{
		perror("getcwd");
		exit(1);
	}
	shlvl = get_environment_variable(shell->env, "SHLVL");
	if (shlvl)
	{
		level = atoi(shlvl) + 1;
		new_shlvl = int_to_str(level);
		if (new_shlvl)
		{
			ft_setenv(shell, "SHLVL", new_shlvl, 1);
			gc_remove_ptr(new_shlvl);
		}
	}
	else
		ft_setenv(shell, "SHLVL", "1", 1);
	setup_signals();
}
