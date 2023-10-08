/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:54:16 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/06 18:54:16 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_single_token(t_tokens *tkn)
{
	if (DEBUG)
	{
		ft_printf("content = %s\n", tkn->content);
		ft_printf("cmd_arr :\n_________________\n");
		print_arr(tkn->cmd_arr);
		ft_printf("_________________\n");
		ft_printf("fd_in = %d, fd_out = %d\n", tkn->fd_in, tkn->fd_out);
	}
}

void	print_tokens_dlist(t_tokens *tok)
{
	t_tokens	*temp;

	temp = tok;
	while (temp)
	{
		ft_printf("DLST FROM CONTENT %s, fd_in = %d, fd_out = %d :\n", temp->content, temp->fd_in, temp->fd_out);
		print_dlist(temp->dlst);
		temp = temp->next;
	}
}

void	print_lst_arr(t_tokens *lst)
{
	t_tokens	*temp;
	size_t		i;

	i = 1;
	temp = lst;
	while (temp)
	{
		ft_printf("array %d from \"%s\" : \n", i, temp->content);
		print_arr(temp->cmd_arr);
		ft_printf("\n");
		temp = temp->next;
		i++;
	}
}

void	print_dlist(t_dlist	*lst)
{
	t_dlist	*temp;

	temp = lst;
	while (temp)
	{
		ft_printf("%s\n", temp->content);
		temp = temp->next;
	}
	ft_printf("\n");
}

void print_tkns_up(t_tokens *lst)
{
	t_tokens *temp;

	if (!lst)
		return;
	temp = lst;
	while (temp->next)
		temp = temp->next;
	while (temp)
	{
		ft_printf("position = %d\n", temp->position);
		ft_printf("%s\n", temp->content);
		temp = temp->prev;
	}
}

void print_tkns_down(t_tokens *lst)
{
	t_tokens *temp;

	if (!lst)
		return;
	temp = lst;
	if (DEBUG)
	{
		while (temp)
		{
			ft_printf("content = %s\n", temp->content);
			ft_printf("type = %d\n", temp->type);
			ft_printf("position = %d\n", temp->position);
			ft_printf("fd_in = %d\n", temp->fd_in);
			ft_printf("fd_out = %d\n\n", temp->fd_out);
			temp = temp->next;
		}
	}
}