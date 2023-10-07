/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:11:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/20 19:29:00 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

static int	replace_content(t_envp **envp, char *name, char *new_content)
{
	t_envp	*temp;

	temp = *envp;
	while (temp)
	{
		if (ft_strcmp(temp->var.name, name) == 0)
			break ;
		temp = temp->next;
	}
	if (temp)
	{
		if (temp->var.content)
			free(temp->var.content);
		if (new_content)
		{
			temp->var.content = ft_strdup(new_content);
			if (!temp->var.content)
				return (0);
		}
		else
		{
			temp->var.content = NULL;
			temp->var.readable = 0;
		}
	}
	return (1);
}

static void	mod_pwd(t_mshell *shell)
{
	char	*pwd;
	char	path[PATH_MAX];

	pwd = get_envvar_content(shell, shell->envp, "PWD");
	if (!replace_content(&shell->envp, "OLDPWD", pwd))
		return (free(pwd), free_struct(shell), exit(2));
	free(pwd);
	if (!getcwd(path, PATH_MAX))
	{
		ft_dprintf(2, "cd: error retrieving current directory: getcwd: ");
		ft_dprintf(2, "cannot access parent directories: ");
		ft_dprintf(2, "No such file or directory\n");
		return ;
	}
	if (!replace_content(&shell->envp, "PWD", path))
		return (free_struct(shell), exit(2));
}


static void	get_error(char **cmd)
{
	struct stat	info;

	if (stat(cmd[1], &info) == -1)
		builtin_error(cmd[0], cmd[1], 1);
	else if (!(info.st_mode & S_IXUSR))
		builtin_error(cmd[0], cmd[1], 4);
	else
		builtin_error(cmd[0], cmd[1], 5);
}

static int	get_dir(t_mshell *shell, char **cmd)
{
	if (cmd[1][0] == '\0')
			return (0);
	if (chdir(cmd[1]))
		get_error(cmd);
	else
		mod_pwd(shell);
	return (1);
}

void cd_case(t_mshell *shell, char **cmd)
{
	char	*home;
	int		arg_count;

	if (check_option(shell, cmd))
		return ;
	arg_count = ft_arr_size(cmd);
	if (arg_count > 1)
	{
		if (arg_count > 2)
			return (builtin_error(cmd[0], cmd[1], 3));
		get_dir(shell, cmd);
		return ;
	}
	home = get_envvar_content(shell, shell->envp, "HOME");
	if (!home)
		return (free(home), builtin_error(cmd[0], cmd[1], 2));
	else if (chdir(home))
		builtin_error(cmd[0], home, 1);
	else
		mod_pwd(shell);
	free(home);
}
