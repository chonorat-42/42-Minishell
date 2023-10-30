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

extern long long	g_status;

void	parent_management(t_tokens *temp, pid_t child, size_t i, t_pipe *data)
{
	data->lpids[i] = child;
	if (temp->prev != NULL)
	{
		close(data->fd[1][0]);
		close(data->fd[1][1]);
	}
	if (temp->next)
	{
		data->fd[1][0] = data->fd[0][0];
		data->fd[1][1] = data->fd[0][1];
	}
}

static void	child_fd(t_mshell *shell, t_tokens *temp, t_pipe *data)
{
	if (temp->prev != NULL)
	{
		dup2(data->fd[1][0], 0);
		close(data->fd[1][0]);
		close(data->fd[1][1]);
	}
	if (temp->next)
	{
		close(data->fd[0][0]);
		dup2(data->fd[0][1], 1);
		close(data->fd[0][1]);
	}
	if (has_bad_fd(temp))
	{
		close_std_fds();
		free_struct(shell);
		free(data->lpids);
		exit(1);
	}
	if (temp->next && temp->next->fd_in != 0)
	{
		free(data->lpids);
		exit(g_status);
	}
	manage_fd(temp->fd_in, temp->fd_out);
}

void	child_management(t_mshell *shell, t_tokens *temp, t_pipe *data)
{
	child_fd(shell, temp, data);
	if (is_builtin(temp))
	{
		builtin_forwarding_pipe(temp, shell);
		free_struct(shell);
		free(data->lpids);
		close_std_fds();
		exit(g_status);
	}
	else
	{
		bin_exec(shell, temp->cmd_arr, data->lpids);
		free(data->lpids);
	}
}

int	fork_pipe(t_mshell *shell, t_tokens **temp, int i, t_pipe *data)
{
	pid_t	child;

	if ((*temp)->next)
		pipe(data->fd[0]);
	child = fork();
	if (child == -1)
		return (0);
	if (!child)
		child_management(shell, *temp, data);
	else
	{
		parent_management(*temp, child, i, data);
		if ((*temp)->has_bad_fd)
		{
			print_errors_single(*temp);
			close(data->fd[0][0]);
			close(data->fd[0][1]);
		}
		if ((*temp)->next && (*temp)->next->type == PIPE)
			*temp = (*temp)->next->next;
		else
			*temp = (*temp)->next;
	}
	return (1);
}

void	handle_pipes(t_mshell *shell, t_tokens *temp)
{
	t_pipe	data;
	size_t	index[2];

	data.fd[1][0] = 0;
	data.fd[1][1] = 1;
	data.lpids = malloc(sizeof(int) * (count_successive_pipes(temp) + 1));
	if (!data.lpids)
		return (free_struct(shell), exit(1));
	index[0] = 0;
	index[1] = 0;
	while (temp)
		if (!fork_pipe(shell, &temp, index[0]++, &data))
			return (free_struct(shell), free(data.lpids), exit(1));
	while (index[1] < (count_successive_pipes(shell->tok_lst) + 1))
	{
		if (waitpid(data.lpids[index[1]], (int *)&g_status, 0) == -1)
			return (perror("waitpid"), exit(EXIT_FAILURE));
		get_fork_status();
		index[1]++;
	}
	return (free(data.lpids), close_all_fds_parent(shell->tok_lst, &data));
}
