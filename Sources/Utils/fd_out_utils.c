/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_out_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:54:58 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/26 15:05:52 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	simple_out_case(t_dlist *temp, int *has_fd, int *temp_fd, t_tokens *tok)
{
	char	*trim;

	ft_dprintf(2, "in simple out case, temp = %s\n\n", tok->content);
	if (*has_fd && *temp_fd != -1)
		close(*temp_fd);
	if (is_char_in_set(temp->next->content[0], "\'\""))
	{
		trim = remove_quotes(temp->next->content);
		free(temp->next->content);
		temp->next->content = trim;
	}
	*temp_fd = open(temp->next->content, O_RDWR | O_CREAT, 0666);
	(*has_fd)++;
	if (*temp_fd == -1)
	{
		fdout_access(temp->next->content);
		ft_printf("!fd_out access, content = %s, tok_has_badfd = %d\n", tok->content, tok->has_bad_fd);
		tok->has_bad_fd++;
	}
}

void	append_case(t_dlist *temp, int *has_fd, int *temp_fd)
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
	if (!fdin_access(temp->next->content))
		ft_printf(":OK\n");
	*temp_fd = open(temp->next->content, O_RDWR | O_APPEND | O_CREAT, 0666);
	(*has_fd)++;
}

void	handle_simple_out(t_fdhandler *handler)
{
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	handler->tok->fd_out_str = handler->fd_str;
	simple_out_case(handler->dlist, &handler->has_fd, &handler->temp_fd, handler->tok);
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

void	handle_append(t_fdhandler *handler)
{
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	append_case(handler->dlist, &handler->has_fd, &handler->temp_fd);
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
