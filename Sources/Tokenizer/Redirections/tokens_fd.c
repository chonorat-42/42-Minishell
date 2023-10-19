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

void	heredoc_case(t_mshell *shell, t_tokens **tok, t_dlist *temp, int *has_fd, int *temp_fd)
{
	if (*has_fd && *temp_fd != -1)
		close(*temp_fd);
	*temp_fd = open("/tmp/temp.heredoc2", O_RDWR | O_CREAT | O_TRUNC, 0666);
	heredoc(shell, temp->next->content, *temp_fd);
	heredoc_into_infile(&(*tok)->dlst);
	close(*temp_fd);
	get_fd_in(shell, tok);
}

void	get_fd_in(t_mshell *shell, t_tokens **tok)
{
	t_tokens	*temp_tok;
	t_dlist		*temp_dlst;
	int			temp_fd;
	int			has_fd;
	char		*fd_str;

	temp_tok = *tok;
	while (temp_tok)
	{
		temp_dlst = temp_tok->dlst;
		has_fd = 0;
		temp_fd = 0;
		while (temp_dlst)
		{
			if (temp_dlst->content[0] == '<'
				&& ft_strlen(temp_dlst->content) == 1)
				{
					fd_str = remove_quotes(temp_dlst->next->content);
					simple_in_case(temp_dlst, &has_fd, &temp_fd);
				}
			else if (temp_dlst->content[0] == '<'
				&& ft_strlen(temp_dlst->content) == 2)
				{
					fd_str = remove_quotes(temp_dlst->next->content);
					heredoc_case(shell, tok, temp_dlst, &has_fd, &temp_fd);
				}
			if (handle_fd(temp_fd, fd_str, CMD))
				return (free_struct(shell), free(shell->input), free_arr(shell->paths), shell->paths = NULL, (get_input_loop(shell)));
			temp_dlst = temp_dlst->next;
		}
		temp_tok->fd_in = temp_fd;
		temp_tok->fd_in_str = fd_str;
		temp_tok = temp_tok->next;
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
	*temp_fd = open(temp->next->content, O_RDWR | O_APPEND);
	(*has_fd)++;
}

static void	get_fd_out(t_tokens **tok)
{
	t_tokens	*temp_tok;
	t_dlist		*temp_dlst;
	int			temp_fd;
	int			has_fd;
	char		*fd_str;

	temp_tok = *tok;
	while (temp_tok)
	{
		temp_dlst = temp_tok->dlst;
		has_fd = 0;
		temp_fd = 1;
		while (temp_dlst)
		{
			if (temp_dlst->content[0] == '>'
				&& ft_strlen(temp_dlst->content) == 1)
				{
					fd_str = remove_quotes(temp_dlst->next->content);
					simple_out_case(temp_dlst, &has_fd, &temp_fd);
					handle_fd(temp_fd, fd_str, CMD);
				}
			else if (temp_dlst->content[0] == '>'
				&& ft_strlen(temp_dlst->content) == 2)
				{
					fd_str = remove_quotes(temp_dlst->next->content);
					append_case(temp_dlst, &has_fd, &temp_fd);
					handle_fd(temp_fd, fd_str, CMD);
				}
				else
					fd_str = NULL;
				
			temp_dlst = temp_dlst->next;
		}
		temp_tok->fd_out = temp_fd;
		temp_tok->fd_out_str = fd_str;
		temp_tok = temp_tok->next;
	}
}

void	get_fds(t_mshell *shell, t_tokens **lst)
{
	get_fd_in(shell, lst);
	get_fd_out(lst);
}
