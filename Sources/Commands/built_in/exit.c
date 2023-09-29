/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:59:01 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/30 00:24:11 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void exit_error(int error, char *cmd)
{
	if (error == 1)
		ft_printf("minishell: exit: %s: numeric argument required\n", cmd);
	else if (error == 2)
		ft_printf("minishell: exit: too many arguments\n");
}

void	exit_case(t_mshell *shell, char **cmd)
{
	long long	exit_value;
	char		*value_str;

	ft_putendl_fd("exit", 1);
	if (count_arr_size(cmd) > 1)
	{
		value_str = get_exit(cmd[1]);
		if (!only_digit(value_str, cmd))
		{
			exit_error(1, cmd[1]);
			exit_value = 2;
		}
		else if (count_arr_size(cmd) > 2)
			return (g_status = 1, exit_error(2, cmd[1]));
		else
		{
			value_str = ft_strdup(cmd[1]);
			exit_value = ll_atoi(value_str);
			free(value_str);
		}
	}
	else
		exit_value = shell->exit_status;
	return (free_struct(shell), exit(exit_value % 256));
}
