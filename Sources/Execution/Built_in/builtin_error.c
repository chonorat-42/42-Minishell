/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:17:36 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/09 15:04:05 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long g_status;

static void	exit_error(char *arg, int error)
{
	if (error == 1)
		ft_dprintf(2, "%s: numeric argument required\n", arg);
	else if (error == 2)
	{
		ft_dprintf(2, "too many arguments\n");
		g_status = 1;
	}
}

static void	cd_error(char *arg, int error)
{
	if (error == 1)
		ft_dprintf(2, "%s: No such file or directory\n", arg);
	if (error == 2)
		ft_dprintf(2, "HOME not set\n");
	if (error == 3)
		ft_dprintf(2, "too many arguments\n");
	if (error == 4)
	{
		ft_dprintf(2, "%s: Permission denied\n", arg);
		g_status = 126;
		return ;
	}
	if (error == 5)
		ft_dprintf(2, "%s: Not a directory\n", arg);
	g_status = 1;
}
static void	option_error(char *cmd, char *arg)
{
	ft_dprintf(2, "%s: options are not supported\n", arg);
	if (ft_strcmp(cmd, "export") == 0)
		ft_dprintf(2, "%s: usage: %s [name[=value] ...]\n", cmd, cmd);
	if (ft_strcmp(cmd, "unset") == 0)
		ft_dprintf(2, "%s: usage: %s [name ...]\n", cmd);
	if (ft_strcmp(cmd, "cd") == 0)
		ft_dprintf(2, "%s: usage: %s [dir]\n", cmd);
	g_status = 2;
}

void	builtin_error(char *cmd, char *arg, int error)
{
	ft_dprintf(2, "minishell: %s: ", cmd);
	if (error == -1)
		return (option_error(cmd, arg));
	if (ft_strcmp(cmd, "export") == 0)
		return (g_status = 1, 
			(void)ft_dprintf(2, "`%s': not a valid identifier\n", arg));
	else if (ft_strcmp(cmd, "exit") == 0)
		return (exit_error(arg, error));
	else if (ft_strcmp(cmd, "cd") == 0)
		return (cd_error(arg, error));
	else if (ft_strcmp(cmd, "unset") == 0)
		return (g_status = 1, 
			(void)ft_dprintf(2, "`%s': not a valid identifier\n", arg));
}