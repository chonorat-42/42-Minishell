/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_out_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:54:58 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/27 15:46:51 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		handler->temp_fd = open(handler->dlist->next->content, O_WRONLY | O_CREAT, 0666);
	else if (token == APPEND)
		handler->temp_fd = open(handler->dlist->next->content,
			O_WRONLY | O_APPEND | O_CREAT, 0666);
	handler->has_fd++;
	return (1);
}

int	handle_simple_out(t_fdhandler *handler)
{
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	handler->tok->fd_out_str = handler->fd_str;
	if (!out_case(handler, RCHEVRON))
		return (0);
	if (handle_fd(handler->temp_fd, handler->fd_str, CMD, handler->tok))
	{
		handler->tok->has_bad_fd++;
		if (!cmd_has_pipes(handler->shell->tok_lst))
			return (print_errors(handler->shell->tok_lst),
				free_tokens(&handler->shell->tok_lst),
				free_arr(handler->shell->paths), handler->shell->paths = NULL,
				free(handler->shell->input), get_input_loop(handler->shell), 0);
	}
	return (1);
}

int	handle_append(t_fdhandler *handler)
{
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	if (!out_case(handler, APPEND))
		return (0);
	if (handle_fd(handler->temp_fd, handler->fd_str, CMD, handler->tok))
	{
		handler->tok->has_bad_fd++;
		if (!cmd_has_pipes(handler->shell->tok_lst))
			return (print_errors(handler->shell->tok_lst),
				free_tokens(&handler->shell->tok_lst),
				free_arr(handler->shell->paths), handler->shell->paths = NULL,
				free(handler->shell->input), get_input_loop(handler->shell), 0);
	}
	return (1);
}
