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
	char	path[PATH_MAX];

	shell->current_loc = getcwd(path, PATH_MAX);
	if (!shell->current_loc)
		return (free_struct(shell), exit(2));
}

void	pwd_case(t_mshell *shell, int fd)
{
	char	path[PATH_MAX];

	shell->current_loc = getcwd(path, PATH_MAX);
	if (!shell->current_loc)
		shell->current_loc = NULL;
	else if (shell->current_loc[0])
		ft_dprintf(fd, "%s", shell->current_loc);
	ft_dprintf(fd, "\n");
}
