/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:59:01 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/25 23:52:34 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	exit_case(t_mshell *shell, char *cmd)
{
	long long	exit_value;
	char		*value_str;

	ft_putendl_fd("exit", 1);
	if (ft_strlen(cmd) > 5)
	{
		value_str = get_exit(cmd);
		if (!only_digit(value_str))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", &cmd[5]);
			exit_value = 2;
		}
		else
		{
			value_str = ft_strdup(&cmd[5]);
			exit_value = ll_atoi(value_str);
			free(value_str);
		}
	}
	else
		exit_value = shell->exit_status;
	return (free_struct(shell), exit(exit_value % 256));
}
