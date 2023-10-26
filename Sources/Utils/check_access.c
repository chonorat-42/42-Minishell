/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 14:01:43 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/26 16:03:38 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

int	check_access(char *path)
{
	struct stat	info;

	stat(path, &info);
	if (access(path, F_OK) == -1)
		return (show_error(path, EXEC, 2), 0);
	if (info.st_mode & S_IFDIR)
		return (show_error(path, EXEC, 3), 0);
	if (!(info.st_mode & S_IRUSR) || !(info.st_mode & S_IXUSR))
		return (show_error(path, PERM, 0), g_status = 126, 0);
	return (1);
}

int	fdin_access(char *path)
{
	struct stat	info;

	stat(path, &info);
	if (access(path, F_OK) == -1)
		return (show_error(path, EXEC, 2), 0);
	if (info.st_mode & S_IFDIR)
		return (show_error(path, EXEC, 3), 0);
	if (!(info.st_mode & S_IRUSR) || !(info.st_mode & S_IXUSR))
		return (show_error(path, PERM, 0), g_status = 1, 0);
	return (1);
}

int	fdout_access(char *path)
{
	struct stat	info;

	stat(path, &info);
	if (!(info.st_mode & S_IRUSR) || !(info.st_mode & S_IXUSR))
		return (show_error(path, PERM, 0), g_status = 1, 0);
	return (1);
}
