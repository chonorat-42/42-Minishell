/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 14:01:43 by chonorat          #+#    #+#             */
/*   Updated: 2023/11/06 12:09:08 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_access(char *path)
{
	struct stat	info;

	stat(path, &info);
	if (access(path, F_OK) == -1)
		return (show_error(path, EXEC, 2), 0);
	else if (info.st_mode & S_IFDIR)
		return (show_error(path, EXEC, 3), 0);
	else if (!(info.st_mode & S_IRUSR) || !(info.st_mode & S_IXUSR))
		return (show_error(path, PERM, 0), g_status = 126, 0);
	return (1);
}

int	fdin_access(char *path)
{
	struct stat	info;

	stat(path, &info);
	if (access(path, F_OK) == -1)
		return (show_error(path, EXEC, 2), 0);
	else if (info.st_mode & S_IFDIR)
		return (show_error(path, EXEC, 3), 0);
	else if (!(info.st_mode & S_IRUSR))
		return (show_error(path, PERM, 0), 0);
	return (1);
}

int	fdout_access(char *path)
{
	struct stat	info;

	stat(path, &info);
	if (access(path, F_OK))
		return (1);
	else if (info.st_mode & S_IFDIR)
		return (show_error(path, EXEC, 3), 0);
	else if (!(info.st_mode & S_IWUSR))
		return (show_error(path, PERM, 0), 0);
	return (1);
}
