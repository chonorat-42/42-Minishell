/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_in_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:58:04 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/11/02 16:04:37 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	simple_in_case(t_fdhandler *handler)
{
	char	*trim;

	if (handler->has_fd && handler->temp_fd != -1)
		close(handler->temp_fd);
	if (is_char_in_set(handler->dlist->next->content[0], "\'\""))
	{
		trim = remove_quotes(handler->dlist->next->content);
		if (!trim)
			return (free_struct(handler->shell), exit(1), 1);
		free(handler->dlist->next->content);
		handler->dlist->next->content = trim;
	}
	if (!fdin_access(handler->dlist->next->content))
	{
		g_status = 1;
		handler->tok->has_bad_fd = 1;
		return (0);
	}
	handler->temp_fd = open(handler->dlist->next->content, O_RDONLY);
	handler->has_fd++;
	return (1);
}

void	heredoc_case(t_fdhandler *handler)
{
	if (handler->has_fd)
	{
		close(handler->temp_fd);
		free(handler->fd_str);
		handler->fd_str = NULL;
	}
	handler->temp_fd = open("/tmp/temp.heredoc2",
			O_RDWR | O_CREAT | O_TRUNC, 0666);
	heredoc(handler->shell, handler->dlist->next->content,
		handler->temp_fd);
	if (handler->tok && handler->tok->dlst)
	{
		if (!heredoc_into_infile(&handler->tok->dlst))
			return (close(handler->temp_fd), free_struct(handler->shell));
	}
	close(handler->temp_fd);
	get_fd_in(handler->shell, &handler->tok);
}

int	handle_simple_in(t_fdhandler *handler)
{
	if (handler->fd_str)
		free(handler->fd_str);
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	if (!simple_in_case(handler))
	{
		bad_fd(handler);
		return (0);
	}
	if (handle_fd(handler->temp_fd, handler->fd_str, CMD, handler->tok))
	{
		handler->tok->has_bad_fd++;
		if (!cmd_has_pipes(handler->shell->tok_lst))
			return (print_errors(handler->shell->tok_lst),
				handler->tok->fd_in_str = NULL,
				free_tokens(&handler->shell->tok_lst),
				free_arr(handler->shell->paths), handler->shell->paths = NULL,
				free(handler->shell->input), get_input_loop(handler->shell), 0);
	}
	return (1);
}

void	handle_heredoc(t_fdhandler *handler)
{
	heredoc_case(handler);
	if (handle_fd(handler->temp_fd, handler->fd_str, CMD, handler->tok))
	{
		handler->tok->has_bad_fd++;
		if (!cmd_has_pipes(handler->shell->tok_lst))
			return (print_errors(handler->shell->tok_lst),
				free_tokens(&handler->shell->tok_lst),
				free_arr(handler->shell->paths), handler->shell->paths = NULL,
				free(handler->shell->input), get_input_loop(handler->shell));
	}
}
