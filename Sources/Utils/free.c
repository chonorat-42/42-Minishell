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

#include "minishell.h"

void	free_arr(char **arr)
{
	int j;

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

void	free_envp(t_envp **head)
{
	t_envp	*temp;
	t_envp	*next;

	temp = *head;
	next = NULL;
	if (*head)
		return ;
	while (temp)
	{
		next = temp->next;
		free(temp->var.name);
		free(temp->var.content);
		free(temp);
		temp = next;
	}
	*head = NULL;
}

void	free_atomic_arr(char **arr, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (arr[i])
			free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_struct(t_mshell *shell)
{
	if (shell->input)
		free(shell->input);
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
	{
		free_atomic_arr(shell->menvp, shell->envp_size);
		shell->menvp = NULL;
	}
	if (shell->envp)
		free_envp(&shell->envp);
	if (shell->export)
		free_envp(&shell->export);
	clear_history();
}