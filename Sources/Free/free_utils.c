/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 19:01:54 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/08 19:01:54 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens_dlist(t_tokens **lst)
{
	t_tokens	*temp;

	temp = *lst;
	while (temp)
	{
		free_dlist(&temp->dlst);
		temp = temp->next;
	}
}

void	free_dlist(t_dlist **head)
{
	t_dlist	*temp;

	if (!*head)
		return ;
	while (*head)
	{
		if ((*head)->content)
			free((*head)->content);
		temp = *head;
		*head = temp->next;
		free(temp);
	}
	*(head) = NULL;
}

void	free_tokens(t_tokens	**head)
{
	t_tokens	*temp;

	while (*head)
	{
		if ((*head)->content)
			free((*head)->content);
		if ((*head)->cmd_arr)
			free_arr((*head)->cmd_arr);
		if ((*head)->dlst)
		{
			free_dlist(&(*head)->dlst);
			(*head)->dlst = NULL;
		}
		if ((*head)->fd_in_str)
			free((*head)->fd_in_str);
		if ((*head)->fd_out != 1)
			free((*head)->fd_out_str);
		if ((*head)->errors)
			free_errors((*head)->errors);
		temp = *head;
		*head = temp->next;
		free(temp);
	}
	free(*head);
	*(head) = NULL;
}

void	free_envp(t_envp **head)
{
	t_envp	*temp;
	t_envp	*next;

	if (!*head)
		return ;
	temp = *head;
	next = NULL;
	while (temp)
	{
		next = temp->next;
		free(temp->var.name);
		if (temp->var.content)
			free(temp->var.content);
		free(temp);
		temp = next;
	}
	*head = NULL;
}

void	free_arr(char **arr)
{
	int	j;

	if (!arr)
		return ;
	j = 0;
	if (!arr)
		return ;
	while (arr[j])
	{
		if (arr[j])
			free(arr[j]);
		j++;
	}
	free(arr);
	arr = NULL;
}
