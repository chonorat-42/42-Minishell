/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:17:36 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/30 14:49:37 by chonorat         ###   ########.fr       */
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
		ft_printf("too many arguments\n");
		g_status = 1;
	}
}

void	builtin_error(char *cmd, char *arg, int error)
{
	(void)error;
	ft_dprintf(2, "minishell: %s: ", cmd);
	if (ft_strcmp(cmd, "export") == 0)
		return (g_status = 1, 
			(void)ft_dprintf(2, "`%s': not a valid identifier\n", arg));
	else if (ft_strcmp(cmd, "exit") == 0)
		return (exit_error(arg, error));
}