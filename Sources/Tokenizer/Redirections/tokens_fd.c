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
	if (*has_fd)
		close(*temp_fd);
	*temp_fd = open(temp->next->content, O_RDWR);
	(*has_fd)++;
}

void	heredoc_case(t_mshell *shell, t_tokens **tok, t_dlist *temp, int *has_fd, int *temp_fd)
{
	if (*has_fd)
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
				simple_in_case(temp_dlst, &has_fd, &temp_fd);
			else if (temp_dlst->content[0] == '<'
				&& ft_strlen(temp_dlst->content) == 2)
				heredoc_case(shell, tok, temp_dlst, &has_fd, &temp_fd);
			temp_dlst = temp_dlst->next;
		}
		temp_tok->fd_in = temp_fd;
		temp_tok = temp_tok->next;
	}
}

void	simple_out_case(t_dlist *temp, int *has_fd, int *temp_fd)
{
	if (*has_fd)
		close(*temp_fd);
	*temp_fd = open(temp->next->content, O_RDWR | O_CREAT, 0666);
	(*has_fd)++;
}

void	append_case(t_dlist *temp, int *has_fd, int *temp_fd)
{
	if (*has_fd)
		close(*temp_fd);
	*temp_fd = open(temp->next->content, O_RDWR | O_APPEND);
	(*has_fd)++;
}

static void	get_fd_out(t_tokens **tok)
{
	t_tokens	*temp_tok;
	t_dlist		*temp_dlst;
	int			temp_fd;
	int			has_fd;

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
				simple_out_case(temp_dlst, &has_fd, &temp_fd);
			else if (temp_dlst->content[0] == '>'
				&& ft_strlen(temp_dlst->content) == 2)
				append_case(temp_dlst, &has_fd, &temp_fd);
			temp_dlst = temp_dlst->next;
		}
		temp_tok->fd_out = temp_fd;
		temp_tok = temp_tok->next;
	}
}

void	get_fds(t_mshell *shell, t_tokens **lst)
{
	get_fd_in(shell, lst);
	get_fd_out(lst);
}
