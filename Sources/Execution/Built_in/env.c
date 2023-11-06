/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:40:23 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/09 17:40:23 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_case(t_mshell *shell, char **cmd, int fd)
{
	t_envp	*temp;

	if (check_option(shell, cmd))
		return ;
	if (cmd[1])
		return (g_status = 1, builtin_error(cmd[0], cmd[1], 0));
	temp = shell->envp;
	while (temp)
	{
		if (temp->var.readable == 1)
		{
			if (ft_putstr_fd(temp->var.name, fd) == -1)
				return (free_struct(shell), exit(1));
			if (ft_putchar_fd('=', fd) == -1)
				return (free_struct(shell), exit(1));
			if (ft_putstr_fd(temp->var.content, fd) == -1)
				return (free_struct(shell), exit(1));
			if (ft_putchar_fd('\n', fd) == -1)
				return (free_struct(shell), exit(1));
		}
		temp = temp->next;
	}
	if (fd != 1)
		close(fd);
}
