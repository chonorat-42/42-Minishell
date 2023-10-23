/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:29 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/23 15:14:28 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

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

void	show_error(char *cmd, char *type, int error)
{
	ft_putstr_fd("\033[0;37m\033[1mminishell: ", 2);
	if (ft_strcmp(type, "EXEC") == 0)
		exec_error(cmd, error);
	else if (ft_strcmp(type, "QUOTES") == 0)
		return (g_status = 1,
			(void)ft_putendl_fd("quotes are not closed\033[0m", 2));
	else if (ft_strcmp(type, "HEREDOC") == 0)
		heredoc_error(cmd);
	else if (ft_strcmp(type, "NO_FILE") == 0)
		return (g_status = 1,
			(void)ft_dprintf(2, "%s: No such file or directory\n\033[0m", cmd));
	else if (ft_strcmp(type, "PERMISSION") == 0)
		ft_dprintf(2, "%s: Permission denied\n\033[0m", cmd);
	else if (ft_strcmp(type, "SYNTAX") == 0)
		return (g_status = 2, (void)ft_dprintf(2,
				"syntax error near unexpected token `%s'\n\033[0m", cmd));
	else if (ft_strcmp(type, "OPERATOR") == 0)
		operator_error(cmd, error);
	else if (ft_strcmp(type, "SHLVL") == 0)
		ft_dprintf(2,
			"warning: shell level (%d) too high, resetting to 1\n\033[0m",
			error);
}
