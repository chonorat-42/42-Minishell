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

int cmd_has_pipes(t_tokens *lst)
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

void	init_fdhandler(t_fdhandler *handler, t_mshell *shell, t_tokens *tokens)
{
	handler->tok = tokens;
	handler->shell = shell;
	handler->fd_str = NULL;
}

void	init_handler_loop(t_fdhandler *handler, int type)
{
	handler->has_fd = 0;
	if (type == 1)
		handler->temp_fd = 1;
	else
		handler->temp_fd = 0;
	handler->dlist = handler->tok->dlst;
}

void	handle_simple_in(t_fdhandler *handler)
{
	handler->fd_str = remove_quotes(handler->dlist->next->content);

	// ft_dprintf(2, "in handle simple in, fd_str = %s\n", handler->fd_str);

	simple_in_case(handler->dlist, &handler->has_fd, &handler->temp_fd);

	// ft_dprintf(2, "out of simple in case\n", handler->fd_str);

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

void	get_fd_in(t_mshell *shell, t_tokens **tok)
{
	t_fdhandler	handler;

	init_fdhandler(&handler, shell, *tok);
	while(handler.tok)
	{
		init_handler_loop(&handler, 0);
		while (handler.dlist)
		{
			if (!ft_strcmp(handler.dlist->content, "<"))
				handle_simple_in(&handler);
			else if (!ft_strcmp(handler.dlist->content, "<<"))
				handle_heredoc(&handler);
			else
			{
				if (handler.fd_str)
					free(handler.fd_str);
				handler.fd_str = NULL;
			}
			handler.dlist = handler.dlist->next;
		}
		handler.tok->fd_in = handler.temp_fd;
		handler.tok->fd_in_str = handler.fd_str;
		handler.tok = handler.tok->next;
	}
}

void	simple_out_case(t_dlist *temp, int *has_fd, int *temp_fd)
{
	char	*trim;

	// ft_dprintf(2, "in simple out case, temp =\n");
	// print_dlist(temp);

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
	*temp_fd = open(temp->next->content, O_RDWR | O_APPEND | O_CREAT, 0666);
	(*has_fd)++;
}

void	handle_simple_out(t_fdhandler *handler)
{
	handler->fd_str = remove_quotes(handler->dlist->next->content);
	simple_out_case(handler->dlist, &handler->has_fd, &handler->temp_fd);
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

void	get_fd_out(t_mshell *shell, t_tokens **tok)
{
	t_fdhandler	handler;

	init_fdhandler(&handler, shell, *tok);
	while(handler.tok)
	{
		init_handler_loop(&handler, 1);
		while (handler.dlist)
		{
			if (!ft_strcmp(handler.dlist->content, ">"))
				handle_simple_out(&handler);
			else if (!ft_strcmp(handler.dlist->content, ">>"))
				handle_append(&handler);
			else
			{
				if (handler.fd_str)
					free(handler.fd_str);
				handler.fd_str = NULL;
			}
			handler.dlist = handler.dlist->next;
		}
		handler.tok->fd_out = handler.temp_fd;
		handler.tok->fd_out_str = handler.fd_str;
		handler.tok = handler.tok->next;
	}
}

void	get_fds(t_mshell *shell, t_tokens **lst)
{
	get_fd_in(shell, lst);
	get_fd_out(shell, lst);
}
