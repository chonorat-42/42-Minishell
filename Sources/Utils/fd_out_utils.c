/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_out_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:54:58 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/11/10 14:23:43 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_fd_out(t_fdhandler *handler)
{
	handler->tok->fd_out = handler->temp_fd;
	if (handler->fd_str)
	{
		handler->tok->fd_out_str = ft_strdup(handler->fd_str);
		if (!handler->tok->fd_out_str)
			return (0);
	}
	free(handler->fd_str);
	handler->fd_str = NULL;
	handler->tok = handler->tok->next;
	return (1);
}

void	handler_end_loop(t_fdhandler *handler, int type)
{
	if (handler->tok)
	{
		if (type == 0)
		{
			handler->tok->fd_in = handler->temp_fd;
			if (handler->fd_str)
			{
				handler->tok->fd_in_str = ft_strdup(handler->fd_str);
				if (!handler->tok->fd_in_str)
					return (free(handler->fd_str), free_struct(handler->shell),
						exit(1));
			}
			free(handler->fd_str);
			handler->fd_str = NULL;
			handler->tok = handler->tok->next;
		}
		else
		{
			if (!get_fd_out(handler))
				return (free(handler->fd_str), free_struct(handler->shell),
					exit(1));
		}
	}
}

static int	out_case(t_fdhandler *handler, int token)
{
	char	*trim;

	if (handler->has_fd && handler->temp_fd != -1)
		close(handler->temp_fd);
	if (is_char_in_set(handler->dlist->next->content[0], "\'\""))
	{
		trim = remove_quotes(handler->dlist->next->content);
		free(handler->dlist->next->content);
		handler->dlist->next->content = trim;
	}
	if (!fdout_access(handler->dlist->next->content))
	{
		g_status = 1;
		handler->tok->has_bad_fd = 1;
		return (0);
	}
	if (token == RCHEVRON)
		handler->temp_fd = open(handler->dlist->next->content,
				O_WRONLY | O_CREAT, 0666);
	else if (token == APPEND)
		handler->temp_fd = open(handler->dlist->next->content,
				O_WRONLY | O_APPEND | O_CREAT, 0666);
	handler->has_fd++;
	return (1);
}

int	handle_simple_out(t_fdhandler *handler)
{
	if (handler->fd_str)
		free(handler->fd_str);
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	if (!handler->fd_str)
		return (free_struct(handler->shell), exit(1), 0);
	if (!out_case(handler, RCHEVRON))
	{
		bad_fd(handler);
		return (0);
	}
	if (handle_fd(handler->temp_fd, handler->fd_str, CMD, handler->tok))
	{
		handler->tok->has_bad_fd++;
		if (!cmd_has_pipes(handler->shell->tok_lst))
			return (print_errors(handler->shell->tok_lst),
				handler->tok->fd_out_str = NULL,
				free_tokens(&handler->shell->tok_lst),
				free_arr(handler->shell->paths), handler->shell->paths = NULL,
				free(handler->shell->input), get_input_loop(handler->shell), 0);
	}
	return (1);
}

int	handle_append(t_fdhandler *handler)
{
	if (handler->fd_str)
		free(handler->fd_str);
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	if (!handler->fd_str)
		return (free_struct(handler->shell), exit(1), 0);
	if (!out_case(handler, APPEND))
	{
		bad_fd(handler);
		return (0);
	}
	if (handle_fd(handler->temp_fd, handler->fd_str, CMD, handler->tok))
	{
		handler->tok->has_bad_fd++;
		if (!cmd_has_pipes(handler->shell->tok_lst))
			return (print_errors(handler->shell->tok_lst),
				handler->tok->fd_out_str = NULL,
				free_tokens(&handler->shell->tok_lst),
				free_arr(handler->shell->paths), handler->shell->paths = NULL,
				free(handler->shell->input), get_input_loop(handler->shell), 0);
	}
	return (1);
}
