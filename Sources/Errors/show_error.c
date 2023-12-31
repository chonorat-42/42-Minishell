/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:29 by chonorat          #+#    #+#             */
/*   Updated: 2023/11/10 00:51:34 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_error(char *cmd, int error)
{
	if (error == 1)
		ft_dprintf(2, "%s: command not found\n\033[0m", cmd);
	else if (error == 2)
		ft_dprintf(2, "%s: No such file or directory\n\033[0m", cmd);
	if (error == 3)
	{
		ft_dprintf(2, "%s: Is a directory\n\033[0m", cmd);
		g_status = 126;
		return ;
	}
	g_status = 127;
}

static void	heredoc_error(char *delimiter)
{
	ft_dprintf(2, "warning: ");
	ft_dprintf(2, "here-document delimited by end-of-file ");
	ft_dprintf(2, "(wanted `%s')\n\033[0m", delimiter);
}

static void	operator_error(char *operator, int error)
{
	if (error == 1)
		ft_dprintf(2, "operator `%s' is not supported\n\033[0m", operator);
	else if (error == 2)
		ft_dprintf(2, "operator `%c' and `%s' are not supported\n",
			operator[0], operator);
	g_status = 1;
}

void	show_error(char *cmd, int type, int error)
{
	ft_putstr_fd("\033[0;37m\033[1mminishell: ", 2);
	if (type == ARGV)
		ft_dprintf(2, "%s: can't take arguments\n\033[0m", cmd);
	if (type == EXEC)
		exec_error(cmd, error);
	else if (type == QUOTES)
		return (g_status = 1,
			(void)ft_putendl_fd("quotes are not closed\033[0m", 2));
	else if (type == HDOC)
		heredoc_error(cmd);
	else if (type == NO_F)
		return (g_status = 1,
			(void)ft_dprintf(2, "%s: No such file or directory\n\033[0m", cmd));
	else if (type == PERM)
		ft_dprintf(2, "%s: Permission denied\n\033[0m", cmd);
	else if (type == SYNTAX)
		return (g_status = 2, (void)ft_dprintf(2,
				"syntax error near unexpected token `%s'\n\033[0m", cmd));
	else if (type == OPERATOR)
		operator_error(cmd, error);
	else if (type == SHLVL)
		ft_dprintf(2,
			"warning: shell level (%d) too high, resetting to 1\n\033[0m",
			error);
}
