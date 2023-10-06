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

int	get_current_location(t_mshell *shell)
{
	char	path[PATH_MAX];

	shell->current_loc = getcwd(path, PATH_MAX);
	if (!shell->current_loc)
	{
		ft_dprintf(2, "error retrieving current directory\n");
		return (0);
	}
	return (1);
}

void	pwd_case(int fd)
{
	char	path[PATH_MAX];

	if (!getcwd(path, PATH_MAX))
		ft_dprintf(2, "pwd: error retrieving current directory");
	else if (path[0])
		ft_dprintf(fd, "%s", path);
	ft_dprintf(fd, "\n");
}
