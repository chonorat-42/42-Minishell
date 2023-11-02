/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:17:36 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/31 11:59:28 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

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
	else if (error == 2)
		ft_dprintf(2, "HOME not set\n");
	else if (error == 3)
		ft_dprintf(2, "too many arguments\n");
	else if (error == 4)
	{
		ft_dprintf(2, "%s: Permission denied\n", arg);
		g_status = 126;
		return ;
	}
	else if (error == 5)
		ft_dprintf(2, "%s: Not a directory\n", arg);
	else if (error == 6)
	{
		ft_dprintf(2, "cd: error retrieving current directory: getcwd: ");
		ft_dprintf(2, "cannot access parent directories: ");
		ft_dprintf(2, "No such file or directory\n");
		return ;
	}
	g_status = 1;
}

static void	option_error(char *cmd, char *arg)
{
	ft_dprintf(2, "%s: options are not supported\n", arg);
	if (ft_strcmp(cmd, "export") == 0)
		ft_dprintf(2, "%s: usage: %s [name[=value] ...]\n", cmd, cmd);
	else if (ft_strcmp(cmd, "unset") == 0)
		ft_dprintf(2, "%s: usage: %s [name ...]\n", cmd, cmd);
	else if (ft_strcmp(cmd, "cd") == 0)
		ft_dprintf(2, "%s: usage: %s [dir]\n", cmd, cmd);
	g_status = 2;
}

void	builtin_error(char *cmd, char *arg, int error)
{
	ft_dprintf(2, "\033[0;37m\033[1mminishell: %s: ", cmd);
	if (error == -1)
		return (option_error(cmd, arg));
	if (ft_strcmp(cmd, "export") == 0)
		return (g_status = 1,
			(void)ft_dprintf(2, "`%s': not a valid identifier\033[0m\n", arg));
	else if (ft_strcmp(cmd, "exit") == 0)
		exit_error(arg, error);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (cd_error(arg, error));
	else if (ft_strcmp(cmd, "unset") == 0)
		return (g_status = 1,
			(void)ft_dprintf(2, "`%s': not a valid identifier\n\033[0m", arg));
	else if (ft_strcmp(cmd, "pwd") == 0)
		ft_dprintf(2, "error retrieving current directory");
	else if (ft_strcmp(cmd, "env") == 0)
		ft_dprintf(2, "%s: arguments are not supported\n", arg);
	ft_printf("\033[0m");
}
