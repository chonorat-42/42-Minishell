/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 10:59:50 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/06 10:59:50 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_fd_lst(t_dlist **lst, char c, t_dlist **new)
{
	t_dlist *temp;

	temp = *lst;
	while (temp)
	{
		if (temp && temp->content[0] == c)
			temp = temp->next;
		else
		{
			split_into_dlst(new, temp->content, ft_strlen(temp->content), 0);
		}
		temp = temp->next;
	}
}

void	remove_redirect(t_tokens **lst)
{
	t_tokens	*temp;
	t_dlist		*new;

	temp = *lst;
	new = NULL;
	while (temp)
	{
		if (temp->fd_in != STDIN_FILENO)
		{
			remove_fd_lst(&temp->dlst, '<', &new);
			free_dlist(&temp->dlst);
			temp->dlst = new;
		}
		if (temp->fd_out != STDOUT_FILENO)
		{
			remove_fd_lst(&temp->dlst, '>', &new);
			free_dlist(&temp->dlst);
			temp->dlst = new;
		}
		temp = temp->next;
	}
}

static void	get_fd_in(t_tokens **tok, t_envp *envp)
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
		temp_fd= 0;

		while (temp_dlst)
		{
			if (temp_dlst->content[0] == '<' && ft_strlen(temp_dlst->content) == 1)
			{
				if (has_fd)
					close(temp_fd);
				temp_fd = open(temp_dlst->next->content, O_RDWR);
				has_fd++;
			}	
			else if (temp_dlst->content[0] == '<' && ft_strlen(temp_dlst->content) == 2)
			{
				if (has_fd)
					close(temp_fd);
				temp_fd = open("/tmp/temp.heredoc2", O_RDWR | O_CREAT | O_TRUNC, 0666);
				heredoc(temp_dlst->next->content, temp_fd, envp);
				heredoc_into_infile(&(*tok)->dlst);
				get_fd_in(tok, envp);
			}
			temp_dlst = temp_dlst->next;
		}
		temp_tok->fd_in = temp_fd;
		temp_tok = temp_tok->next;
	}
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
		temp_fd= 1;
		while (temp_dlst)
		{
			if (temp_dlst->content[0] == '>' && ft_strlen(temp_dlst->content) == 1)
			{
				if (has_fd)
					close(temp_fd);
				temp_fd = open(temp_dlst->next->content, O_RDWR | O_CREAT, 0666);
				has_fd++;
			}
			else if (temp_dlst->content[0] == '>' && ft_strlen(temp_dlst->content) == 2)
			{
				if (has_fd)
					close(temp_fd);
				temp_fd = open(temp_dlst->next->content, O_RDWR | O_APPEND);
				has_fd++;
			}
			temp_dlst = temp_dlst->next;
		}
		temp_tok->fd_out = temp_fd;
		temp_tok = temp_tok->next;
	}
}

void	get_fds(t_tokens **lst, t_envp *envp)
{
		get_fd_in(lst, envp);
		get_fd_out(lst);
}