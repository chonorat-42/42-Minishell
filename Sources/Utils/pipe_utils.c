/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:39:54 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/25 17:40:49 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_bad_fd(t_tokens *temp)
{
	if (temp->fd_in == -1 || temp->fd_out == -1)
		return (1);
	return (0);
}

size_t	count_successive_pipes(t_tokens *temp)
{
	size_t	res;

	res = 0;
	while (temp->next)
	{
		if (temp->next->type == PIPE)
		{
			res++;
			if (temp->next->next)
				temp = temp->next->next;
			else
				return (res);
		}
		else
			return (res);
	}
	return (res);
}

void	close_all_fds_parent(t_tokens *lst, t_pipe *data)
{
	t_tokens	*temp;

	close(data->fd[0][0]);
	close(data->fd[0][1]);
	close(data->fd[1][0]);
	close(data->fd[1][1]);
	temp = lst;
	while (temp)
	{
		if (temp->fd_in != 0 && temp->fd_in != -1)
			close(temp->fd_in);
		if (temp->fd_out != 1 && temp->fd_out != -1)
			close (temp->fd_out);
		temp = temp->next;
	}
}

void	close_all_fds_child(t_tokens *lst, t_pipe *data)
{
	t_tokens	*temp;

	close(data->fd[0][0]);
	close(data->fd[0][1]);
	close(data->fd[1][0]);
	close(data->fd[1][1]);
	temp = lst;
	while (temp)
	{
		if (temp->fd_in != 0 && temp->fd_in != -1)
			close(temp->fd_in);
		if (temp->fd_out != 1 && temp->fd_out != -1)
			close (temp->fd_out);
		temp = temp->next;
	}
	close(0);
	close(1);
	close(2);
}
