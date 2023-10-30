/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 10:59:40 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/09 10:59:40 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

void	get_fd_in(t_mshell *shell, t_tokens **tok)
{
	t_fdhandler	handler;

	init_fdhandler(&handler, shell, *tok);
	while (handler.tok)
	{
		init_handler_loop(&handler, 0);
		while (handler.dlist)
		{
			if (!ft_strcmp(handler.dlist->content, "<"))
			{
				if (!handle_simple_in(&handler))
					break ;
			}
			else if (!ft_strcmp(handler.dlist->content, "<<"))
				handle_heredoc(&handler);
			handler.dlist = handler.dlist->next;
		}
		handler.tok->fd_in = handler.temp_fd;
		handler.tok->fd_in_str = handler.fd_str;
		handler.tok = handler.tok->next;
	}
}

void	get_fd_out(t_mshell *shell, t_tokens **tok)
{
	t_fdhandler	handler;

	init_fdhandler(&handler, shell, *tok);
	while (handler.tok)
	{
		init_handler_loop(&handler, 1);
		while (handler.dlist)
		{
			if (!ft_strcmp(handler.dlist->content, ">"))
			{
				if (!handle_simple_out(&handler))
					break ;
			}
			else if (!ft_strcmp(handler.dlist->content, ">>"))
			{
				if (!handle_append(&handler))
					break ;
			}
			handler.dlist = handler.dlist->next;
		}
		handler.tok->fd_out = handler.temp_fd;
		handler.tok->fd_out_str = handler.fd_str;
		handler.tok = handler.tok->next;
	}
}

int	has_redirect(t_tokens *lst, char c)
{
	t_tokens	*temp_t;
	t_dlist		*temp_d;

	temp_t = lst;
	while (temp_t)
	{
		temp_d = temp_t->dlst;
		while (temp_d)
		{
			if (temp_d->content && temp_d->content[0] == c)
				return (1);
			temp_d = temp_d->next;
		}
		temp_t = temp_t->next;
	}
	return (0);
}

void	remove_empty_nodes(t_tokens *lst)
{
	t_tokens	*temp_t;
	t_dlist		*temp_d;

	temp_t = lst;
	while (temp_t)
	{
		temp_d = temp_t->dlst;
		while (temp_d)
		{
			if (!temp_d->content)
			{
				if (!empty_node_found(temp_t, temp_d))
					break ;
			}
			if (temp_d)
				temp_d = temp_d->next;
		}
		temp_t = temp_t->next;
	}
}

void	get_fds(t_mshell *shell, t_tokens **lst)
{
	remove_empty_nodes(*lst);
	g_status = 0;
	if (has_redirect(*lst, '<'))
		get_fd_in(shell, lst);
	if (has_redirect(*lst, '>'))
		get_fd_out(shell, lst);
}
