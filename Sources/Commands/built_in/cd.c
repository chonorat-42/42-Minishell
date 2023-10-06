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
	int		index;
	t_envp	*temp;

	index = 1;
	temp = *envp;
	while (temp)
	{
		if (ft_strcmp(temp->var.name, name) == 0)
			break ;
		index++;
		temp = temp->next;
	}
	if (temp)
	{
		if (temp->var.content)
			free(temp->var.content);
		temp->var.content = ft_strdup(new_content);
		if (!temp->var.content)
			return (0);
	}
	return (1);
}

static void	mod_pwd(t_mshell *shell)
{
	char	*pwd;
	char	path[PATH_MAX];

	pwd = get_envvar_content(shell->envp, "PWD");
	if (!replace_content(&shell->envp, "OLDPWD", pwd))
		return (free(pwd), free_struct(shell));
	free(pwd);
	if (!getcwd(path, PATH_MAX))
		return (free_struct(shell), exit(2));
	if (!replace_content(&shell->envp, "PWD", path))
		return (free_struct(shell), exit(2));
}

void cd_case(t_mshell *shell, char **cmd)
{
	char	*home;
	int		arg_count;

	arg_count = count_arr_size(cmd);
	if (arg_count > 1)
	{
		if (arg_count > 2)
			return (builtin_error(cmd[0], cmd[1], 3));
		if (cmd[1][0] == '\0')
			return ;
		if (chdir(cmd[1]))
			builtin_error(cmd[0], cmd[1], 1);
		else
			mod_pwd(shell);
		return ;
	}
	home = get_envvar_content(shell->envp, "HOME");
	if (!home[0])
		return (free(home), builtin_error(cmd[0], cmd[1], 2));
	if (chdir(home))
		builtin_error(cmd[0], home, 1);
	else
		mod_pwd(shell);
	free(home);
}
