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

int	check_option(t_mshell *shell, char **cmd)
{
	char	*option;

	if (!cmd[1])
		return (0);
	if (cmd[1][0] != '-')
		return (0);
	if (cmd[1][1])
	{
		option = ft_substr(cmd[1], 0, 2);
		if (!option)
			return (free_struct(shell), exit(EXIT_FAILURE), 1);
		builtin_error(cmd[0], option, -1);
		free(option);
		return (1);
	}
	return (0);
}

int	get_current_location(t_mshell *shell)
{
	char	path[PATH_MAX];

	shell->current_loc = getcwd(path, PATH_MAX);
	if (!shell->current_loc)
	{
		ft_dprintf(2, "Error retrieving current directory\n");
		return (0);
	}
	return (1);
}

void	pwd_case(t_mshell *shell, char **cmd, int fd)
{
	char	path[PATH_MAX];

	if (check_option(shell, cmd))
		return ;
	if (!getcwd(path, PATH_MAX))
		ft_dprintf(2, "pwd: error retrieving current directory");
	else if (path[0])
		ft_dprintf(fd, "%s", path);
	ft_dprintf(fd, "\n");
}
