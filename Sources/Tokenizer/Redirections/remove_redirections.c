/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:00:46 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/09 11:03:16 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_fd_lst(t_dlist **lst, char c, t_dlist **new)
{
	t_dlist *temp;

	temp = *lst;
	while (temp)
	{
		if (temp && temp->content[0] == c)
			temp = temp->next;
		else
		{
			split_into_dlst(new, temp->content, ft_strlen(temp->content), 0);
		}
		temp = temp->next;
	}
}

void	remove_redirect(t_tokens **lst)
{
	t_tokens	*temp;
	t_dlist		*new;

	temp = *lst;
	new = NULL;
	while (temp)
	{
		if (temp->fd_in != STDIN_FILENO)
		{
			remove_fd_lst(&temp->dlst, '<', &new);
			free_dlist(&temp->dlst);
			temp->dlst = new;
		}
		if (temp->fd_out != STDOUT_FILENO)
		{
			remove_fd_lst(&temp->dlst, '>', &new);
			free_dlist(&temp->dlst);
			temp->dlst = new;
		}
		temp = temp->next;
	}
}