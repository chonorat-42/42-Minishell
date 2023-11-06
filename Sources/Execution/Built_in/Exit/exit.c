/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:59:01 by chonorat          #+#    #+#             */
/*   Updated: 2023/11/06 12:08:17 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_case(t_mshell *shell, char **cmd)
{
	ft_putendl_fd("exit", 1);
	if (ft_arr_size(cmd) > 1)
	{
		if (!only_digit(cmd[1]))
		{
			builtin_error(cmd[0], cmd[1], 1);
			g_status = 2;
		}
		else if (ft_arr_size(cmd) > 2)
			return (builtin_error(cmd[0], cmd[1], 2));
		else
			g_status = ll_atoi(cmd[1]);
	}
	else
		g_status = shell->exit_status;
	return (free_struct(shell), close_std_fds(), exit(g_status % 256));
}
