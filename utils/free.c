/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:40:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/15 14:38:45 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arr(char **arr)
{
	int j;

	j = 0;
	while (arr[j])
	{
		free(arr[j]);
		j++;
	}
	free(arr);
}

void	ft_free_list(t_list	**head)
{
	t_list	*temp;

	if (!*head)
		return ;
	while (*head)
	{
		if ((*head)->cmd)
			free((*head)->cmd);
		temp = *head;
		*head = temp->next;
		free(temp);
	}
}

void	ft_free_tokens(t_tokens	**head)
{
	t_tokens	*temp;

	if (!*head)
		return ;
	while (*head)
	{
		if ((*head)->content)
			free((*head)->content);
		if ((*head)->cmd_arr)
			free_arr((*head)->cmd_arr);
		temp = *head;
		*head = temp->next;
		free(temp);
	}
}

void	free_struct(t_mshell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
	if (shell->user)
		free(shell->user);
	if (shell->session)
		free(shell->session);
	if (shell->join_user)
		free(shell->join_user);
	if (shell->current_loc)
		free(shell->current_loc);
	if (shell->cmd)
	{
		free_arr(shell->cmd);
		shell->cmd = NULL;
	}
	if (shell->paths)
	{
		free_arr(shell->paths);
		shell->paths = NULL;
	}
	if (shell->temp)
	{
		free_arr(shell->paths);
		shell->temp = NULL;
	}
	if (shell->hist)
		ft_free_list(&shell->hist);
	if (shell->tok_lst)
		ft_free_tokens(&shell->tok_lst);
}