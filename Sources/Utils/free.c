/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:40:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/19 22:39:53 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	free_arr(char **arr)
{
	int j;

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
}

/*void	ft_free_list(t_list	**head)
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
}*/

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
	if (shell->tok_lst)
		ft_free_tokens(&shell->tok_lst);
	if (shell->menvp)
		free_arr(shell->menvp);
}