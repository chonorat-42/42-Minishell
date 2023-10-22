/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 17:26:44 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/22 19:23:01 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	give_type(t_tokens **lst)
{
	t_tokens	*temp;

	temp = *lst;
	while (temp)
	{
		if (temp->content[0] == '|')
			temp->type = PIPE;
		else
			temp->type = CMD;
		temp = temp->next;
	}
}

void	pipe_found(t_mshell *shell, char *str, size_t *i, size_t *j)
{
	create_token(shell, *i, *j, str);
	*j = *i;
	(*i)++;
	create_token(shell, *i, *j, str);
	*j = *i;
}

void	tokens_addback(t_tokens **lst, t_tokens *new)
{
	t_tokens	*temp;

	temp = *lst;
	while (temp->next)
		temp = temp->next;
	new->next = NULL;
	new->prev = temp;
	temp->next = new;
}

void	init_new_token(t_tokens **new)
{
	(*new)->cmd_arr = NULL;
	if ((*new)->content[0] == '|')
		(*new)->type = PIPE;
	else
		(*new)->type = CMD;
	(*new)->fd_in = 0;
	(*new)->fd_out = 1;
	(*new)->fd_in_str = NULL;
	(*new)->fd_out_str = NULL;
	(*new)->dlst = NULL;
}

void	close_fd(t_mshell *shell)
{
	t_tokens	*temp;

	temp = shell->tok_lst;
	while (temp)
	{
		if (temp->fd_in != 0 && temp->fd_in != -1)
			close (temp->fd_in);
		if (temp->fd_out != 1 && temp->fd_out != -1)
			close(temp->fd_out);
		temp = temp->next;
	}
}