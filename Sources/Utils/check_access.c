/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 14:01:43 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/18 15:48:04 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_access(char *path)
{
	struct stat	info;

	stat(path, &info);
	if (access(path, F_OK) == -1)
		return (show_error(path, "EXEC", 2), 0);
	if (info.st_mode & S_IFDIR)
		return (show_error(path, "EXEC", 3), 0);
	if (!(info.st_mode & S_IRUSR) || !(info.st_mode & S_IXUSR))
		return (show_error(path, "PERMISSION", 0), 0);
	return (1);
}