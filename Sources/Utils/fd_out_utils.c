/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_out_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:54:58 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/26 17:40:20 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	simple_out_case(t_dlist *temp, int *has_fd, int *temp_fd, t_tokens **tok)
{
	char	*trim;

	if (*has_fd && *temp_fd != -1)
		close(*temp_fd);
	if (is_char_in_set(temp->next->content[0], "\'\""))
	{
		trim = remove_quotes(temp->next->content);
		free(temp->next->content);
		temp->next->content = trim;
	}
	if (!fdout_access(temp->next->content))
	{
		g_status = 1;
		(*tok)->has_bad_fd = 1;
		return (0);
	}
	*temp_fd = open(temp->next->content, O_RDWR | O_CREAT, 0666);
	(*has_fd)++;
	return (1);
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

int	handle_simple_out(t_fdhandler *handler)
{
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	handler->tok->fd_out_str = handler->fd_str;
	if (!simple_out_case(handler->dlist, &handler->has_fd, &handler->temp_fd, &handler->tok))
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
