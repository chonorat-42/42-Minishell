/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_fd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:35:54 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/26 16:01:47 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bad_fd(t_fdhandler *handler)
{
	if (handler->fd_str)
	{
		free(handler->fd_str);
		handler->fd_str = NULL;
	}
}

int	cmd_has_pipes(t_tokens *lst)
{
	t_tokens	*temp;

	temp = lst;
	while (temp)
	{
		if (temp->type == PIPE)
			return (1);
		temp = temp->next;
	}
	return (0);
}

void	init_fdhandler(t_fdhandler *handler, t_mshell *shell,
	t_tokens *tokens)
{
	handler->tok = tokens;
	handler->shell = shell;
	handler->fd_str = NULL;
}

void	init_handler_loop(t_fdhandler *handler, int type)
{
	handler->has_fd = 0;
	handler->fd_str = NULL;
	if (type == 1)
		handler->temp_fd = 1;
	else
		handler->temp_fd = 0;
	handler->dlist = handler->tok->dlst;
}

int	empty_node_found(t_tokens *temp_t, t_dlist *temp_d)
{
	t_dlist	*to_remove;

	if (temp_d == temp_t->dlst)
	{
		temp_t->dlst = temp_d->next;
		free(temp_d);
		temp_d = temp_t->dlst;
	}
	else if (!temp_d->next)
	{
		temp_d->prev->next = NULL;
		free(temp_d);
		return (1);
	}
	else
	{
		to_remove = temp_d;
		temp_d->prev->next = temp_d->next;
		temp_d = temp_d->prev;
		free(to_remove);
	}
	return (0);
}
