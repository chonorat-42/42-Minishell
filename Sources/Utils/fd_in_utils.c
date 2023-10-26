/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_in_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:58:04 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/25 16:15:09 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	simple_in_case(t_dlist *temp, int *has_fd, int *temp_fd)
{
	char	*trim;

	if (*has_fd && *temp_fd != -1)
		close(*temp_fd);
	if (is_char_in_set(temp->next->content[0], "\'\""))
	{
		trim = ft_strtrim(temp->next->content, "\'\"");
		free(temp->next->content);
		temp->next->content = trim;
	}
	*temp_fd = open(temp->next->content, O_RDWR);
	(*has_fd)++;
}

void	heredoc_case(t_fdhandler *handler)
{
	if (handler->has_fd && handler->temp_fd != -1)
		close(handler->temp_fd);
	handler->temp_fd = open("/tmp/temp.heredoc2",
		O_RDWR | O_CREAT | O_TRUNC, 0666);
	heredoc(handler->shell, handler->dlist->next->content,
		handler->temp_fd);
	heredoc_into_infile(&handler->dlist);
	close(handler->temp_fd);
	get_fd_in(handler->shell, &handler->tok);
}

void	handle_simple_in(t_fdhandler *handler)
{
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	simple_in_case(handler->dlist, &handler->has_fd, &handler->temp_fd);
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