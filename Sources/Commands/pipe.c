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

void	create_child(int fd_in, int fd_out, t_tokens *temp, t_mshell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (fd_in != 0)
		{
			if (dup2(fd_in, 0) == -1)
			{
				perror("dup2");
				return (free_struct(shell), exit(EXIT_FAILURE));
			}
			close(fd_in);
		}
		if (fd_out != 1)
		{
			if (dup2(fd_out, 1) == -1)
			{
				perror("dup2");
				return (free_struct(shell), exit(EXIT_FAILURE));
			}
			close(fd_out);
		}
		exec_forwarding(temp, shell);
		waitpid(pid, NULL, 0);
		exit(0);
	}
}

/*to do
recuper fd_in de la premiere commande
executer la commande et l'envoyer dans le pipe (fd_out)
fd_in->next->next->in = fd_out*/

void	fork_pipes(size_t pipes_nbr, t_tokens **temp, int *fd_in, t_mshell *shell, int *fd_out)
{
	size_t i;
    pid_t child;
    pid_t child_processes[pipes_nbr + 1];

    i = 0;
    while (i < pipes_nbr)
    {
		// ft_printf("in fork pipes, i = %d\ntemp->arr = \n", i);
		// print_arr((*temp)->cmd_arr);
        int pipefd[2];
        pipe(pipefd);

        child = fork();
        if (child == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (child == 0)
        {
            if (*fd_in != 0)
            {
                if (dup2(*fd_in, 0) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(*fd_in);
            }
            if (i < pipes_nbr - 1)
            {
                if (dup2(pipefd[1], 1) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
				close(pipefd[1]);
            }
            close(pipefd[0]);
			*fd_in = pipefd[0];
			*fd_out = pipefd[1];
            exec_forwarding(*temp, shell);
            exit(EXIT_SUCCESS);
        }
        else
        {
            close(pipefd[1]);
        	child_processes[i] = child;
        }
        (*temp)->next->next->fd_in = pipefd[0];

        if ((*temp)->next->next)
            (*temp) = (*temp)->next->next;

        i++;
    }

    // Wait for all child processes to finish
    for (i = 0; i < pipes_nbr; i++)
    {
        waitpid(child_processes[i], NULL, 0);
    }

    exec_forwarding(*temp, shell);
}

void	fork_pipesB(size_t pipes_nbr, t_tokens **temp, int fd_in, t_mshell *shell, int fd_out)
{
	size_t	i;
	int		pipefd[2];

	(void)fd_out;
	i = 0;
	while (i < pipes_nbr)
	{
		pipe(pipefd);
		create_child(fd_in, pipefd[1], *temp, shell);
		close(pipefd[1]);
		fd_in = pipefd[0];
		if ((*temp)->next->next)
			(*temp) = (*temp)->next->next;
		i++;
	}
	exec_forwarding(*temp, shell);
}

void	handle_pipes(t_mshell *shell, t_tokens **temp, int *fd_in, int *fd_out)
{
	size_t	pipes_nbr;

	pipes_nbr = count_successive_pipes(*temp);
	fork_pipes(pipes_nbr, temp, fd_in, shell, fd_out);
}