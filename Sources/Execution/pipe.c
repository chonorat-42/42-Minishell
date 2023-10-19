/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 15:21:38 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/25 15:21:38 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_successive_pipes(t_tokens *temp)
{
	size_t	res;

	res = 0;
	while (temp->next)
	{
		if (temp->next->type == PIPE)
		{
			res++;
			if (temp->next->next)
				temp = temp->next->next;
			else
				return (res);
		}
		else
			return (res);
	}
	return (res);
}

void	close_all_fds(t_tokens *lst, int *old_fd)
{
	t_tokens	*temp;

	close(old_fd[0]);
	close(old_fd[1]);
	temp = lst;
	while (temp)
	{
		if (temp->fd_in != 0 && temp->fd_in != -1)
			close(temp->fd_in);
		if (temp->fd_out != 1 && temp->fd_out != -1)
			close (temp->fd_out);
		temp = temp->next;
	}
}

void	parent_management(t_mshell *shell, t_tokens *temp, pid_t child, int *lpids, size_t i, int *new_fd, int *old_fd)
{
	lpids[i] = child;

	(void)shell;
	(void)temp;
	if (i != 0)
	{
		close(old_fd[0]);
		close(old_fd[1]);			
	}
	// if (temp->next)
	// {
		old_fd[0] = new_fd[0];
		old_fd[1] = new_fd[1];
	// }
}

void	child_management(t_mshell *shell, t_tokens *temp, int *new_fd, int *old_fd)
{

	(void)old_fd;
	if (temp != shell->tok_lst)
	{
		dup2(old_fd[0], 0);
		close(old_fd[0]);
		close(old_fd[1]);
	}
	if (temp->next)
	{
		close(new_fd[0]);
		dup2(new_fd[1], 1);
		close(new_fd[1]);
	}
	manage_fd(temp->fd_in, temp->fd_out);
	if (is_builtin(temp))
		builtin_forwarding(temp, shell);
	else
		bin_exec(shell, temp->cmd_arr);
}

void	handle_pipes(t_mshell *shell, t_tokens *temp)
{
	int		new_fd[2];
	int		old_fd[2];
	pid_t	child;
	int		*lpids;
	size_t	i;
	size_t	pipes_nbr;
	size_t	j = 0;
	size_t	cmd_nbr;

	pipes_nbr = count_successive_pipes(temp);
	cmd_nbr = pipes_nbr + 1;
	lpids = malloc(sizeof(int) * cmd_nbr);
	i = 0;
	while (temp)
	{
		if (temp->next)
			pipe(new_fd);
		child = fork();
		if (!child)
			child_management(shell, temp, new_fd, old_fd);
		else
			parent_management(shell, temp, child, lpids, i, new_fd, old_fd);
		if (temp->next && temp->next->type == PIPE)
			temp = temp->next->next;
		else
			temp = temp->next;
		i++;
	}
	if (cmd_nbr > 2)
	{
		close(old_fd[0]);
		close(old_fd[1]);
	}
	while (j < pipes_nbr)
	{
		waitpid(lpids[j], NULL, 0);
		j++;
	}
	free(lpids);
	close_all_fds(shell->tok_lst, old_fd);
}
