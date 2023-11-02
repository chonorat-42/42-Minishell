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

void	free_arrays_shell(t_mshell *shell)
{
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
	if (shell->menvp)
	{
		free_arr(shell->menvp);
		shell->menvp = NULL;
	}
	if (shell->exit)
	{
		free_arr(shell->exit);
		shell->exit = NULL;
	}
}

void	free_strings_shell(t_mshell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
}

void	close_std_fds(void)
{
	close(0);
	close(1);
	close(2);
}

void	close_fd_final(t_tokens *lst)
{
	t_tokens	*temp;

	temp = lst;
	while (temp)
	{
		if (temp->fd_in != STDIN_FILENO && temp->fd_in != -1)
			close(temp->fd_in);
		if (temp->fd_out != STDOUT_FILENO && temp->fd_out != -1)
			close(temp->fd_out);
		temp = temp->next;
	}
}

void	free_struct(t_mshell *shell)
{
	close_fd_final(shell->tok_lst);
	free_handler(&shell->handler);
	free_strings_shell(shell);
	free_arrays_shell(shell);
	if (shell->tok_lst)
		free_tokens(&shell->tok_lst);
	if (shell->envp)
		free_envp(&shell->envp);
	if (shell->export)
		free_envp(&shell->export);
	clear_history();
}
