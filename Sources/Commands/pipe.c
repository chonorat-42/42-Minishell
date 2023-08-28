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
	}
	return (res);
}

void	create_child(int fd_in, int fd_out, t_tokens *temp, t_mshell *shell)
{
	pid_t	pid;

	ft_printf("got in create child, in = %d, out = %d\n\n", fd_in, fd_out);
	pid = fork();
	if (pid == 0)
	{
		if (fd_in != 0)
		{
			dup2(fd_in, 0);
			close(fd_in);
		}
		if (fd_out != 1)
		{
			dup2(fd_out, 1);
			close(fd_out);
		}
		ft_printf("in child , bf exec_forwarding\n\n");
		exec_forwarding(temp, shell, 0, 1);
		waitpid(pid, NULL, 0);
		exit(0);
	}
}

void	fork_pipes(size_t pipes_nbr, t_tokens **temp, int fd_in, t_mshell *shell)
{
	size_t	i;
	int		pipefd[2];

	ft_printf("got in fork pipes\n\n");
	i = 0;
	while (i < pipes_nbr)
	{
		pipe(pipefd);
		create_child(fd_in, pipefd[1], *temp, shell);
		close (pipefd[1]);
		fd_in = pipefd[0];
		if ((*temp)->next->next)
			(*temp) = (*temp)->next->next;
		i++;
	}
	if (fd_in != 0)
		dup2(fd_in, 0);
	exec_forwarding(*temp, shell, fd_in, pipefd[1]);
}

void	handle_pipes(t_mshell *shell, t_tokens **temp, int fd_in)
{
	size_t	pipes_nbr;

	pipes_nbr = count_successive_pipes(*temp);
	ft_printf("in handle pipes, pipes_nbr = %d, in = %d\n\n", pipes_nbr, fd_in);
	fork_pipes(pipes_nbr, temp, fd_in, shell);
	// return (free_struct(shell), exit(0));
}