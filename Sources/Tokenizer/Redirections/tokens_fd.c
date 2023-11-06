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

void	get_fd_in(t_mshell *shell, t_tokens **tok)
{
	init_fdhandler(&shell->handler, shell, *tok);
	while (shell->handler.tok)
	{
		init_handler_loop(&shell->handler, 0);
		while (shell->handler.dlist)
		{
			if (!ft_strcmp(shell->handler.dlist->content, "<")
				&& !handle_simple_in(&shell->handler))
				break ;
			else if (!ft_strcmp(shell->handler.dlist->content, "<<"))
				handle_heredoc(&shell->handler);
			else if (shell->handler.dlist->content[0] == '<'
				&& ft_strlen(shell->handler.dlist->content) > 2)
			{
				show_error("<<", SYNTAX, 0);
				break ;
			}
			if (shell->handler.dlist)
				shell->handler.dlist = shell->handler.dlist->next;
		}
		handler_end_loop(&shell->handler, 0);
	}
	if (shell->handler.fd_str)
		free(shell->handler.fd_str);
}

void	get_fd_out(t_mshell *shell, t_tokens **tok)
{
	init_fdhandler(&shell->handler, shell, *tok);
	while (shell->handler.tok)
	{
		init_handler_loop(&shell->handler, 1);
		while (shell->handler.dlist)
		{
			if (!ft_strcmp(shell->handler.dlist->content, ">")
				&& !handle_simple_out(&shell->handler))
				break ;
			else if (!ft_strcmp(shell->handler.dlist->content, ">>")
				&& !handle_append(&shell->handler))
				break ;
			else if (shell->handler.dlist->content[0] == '>'
				&& ft_strlen(shell->handler.dlist->content) > 2)
			{
				show_error(">>", SYNTAX, 0);
				break ;
			}
			shell->handler.dlist = shell->handler.dlist->next;
		}
		handler_end_loop(&shell->handler, 1);
	}
	if (shell->handler.fd_str)
		free(shell->handler.fd_str);
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
