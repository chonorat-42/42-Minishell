/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:16:14 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/20 18:51:16 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void env_case(t_mshell *shell, char **cmd, int fd)
{
	t_envp	*temp;

	if (check_option(shell, cmd))
		return ;
	if (cmd[1])
	{
		builtin_error(cmd[0], cmd[1], 0);
		return ;
	}
	temp = shell->envp;
	while (temp)
	{
		if (temp->var.readable == 1)
		{
			ft_putstr_fd(temp->var.name, fd);
			ft_putchar_fd('=', fd);
			ft_putendl_fd(temp->var.content, fd);
		}
		temp = temp->next;
	}
}
