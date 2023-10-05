/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 22:44:37 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/31 16:34:29 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void get_current_location(t_mshell *shell)
{
	 shell->current_loc = getcwd(NULL, 0);
	 if (!shell->current_loc)
		ft_printf("PWD");
}

void	pwd_case(t_mshell *shell, int fd)
{
	get_current_location(shell);
	if (shell->current_loc[0])
		ft_dprintf(fd, "%s", shell->current_loc);
	ft_dprintf(fd, "\n");
	if (shell->current_loc)
	{
		free(shell->current_loc);
		shell->current_loc = NULL;
	}
}
