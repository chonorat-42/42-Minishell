/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 16:56:10 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/24 14:15:56 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_exec(char *cmd)
{
	size_t	j;
	char	*res;

	j = 0;
	while (cmd[j])
		j++;
	while (j != 0 && cmd[j] != '/')
		j--;
	j++;
	res = ft_substr(cmd, j, ft_strlen(cmd));
	return (res);
}

char	*get_path(char *cmd)
{
	size_t	i;
	char	*res;

	i = 0;
	while (cmd[i])
		i++;
	while (cmd[i] != '/')
		i--;
	i++;
	res = ft_substr(cmd, 0, i);
	return (res);
}

void	manage_fd(int fd_in, int fd_out)
{
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
}
