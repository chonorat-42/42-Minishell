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
	arr = NULL;
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
		if ((*head)->dlst)
			free_dlist(&(*head)->dlst);
		temp = *head;
		*head = temp->next;
		free(temp);
	}
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
		free_arr(shell->menvp);
	if (shell->envp)
		free_envp(&shell->envp);
	if (shell->export)
		free_envp(&shell->export);
	clear_history();
}