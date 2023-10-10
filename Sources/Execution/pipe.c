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
	if (DEBUG)
		ft_printf("number of successive pipes : %d\n\n", res);
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

int	get_last_out(t_tokens *lst)
{
	t_tokens	*temp;

	temp = lst;
	while (temp->next)
		temp = temp->next;
	return (temp->fd_out);
}

/*to do
recuper fd_in de la premiere commande
executer la commande et l'envoyer dans le pipe (fd_out)
fd_in->next->next->in = fd_out
tant que sur un pipe, pour le dernier envoyer last_out*/

// void	fork_pipes(size_t pipes_nbr, t_tokens **temp, int *fd_in, t_mshell *shell, int *fd_out)
// {
// 	int		pipefd[2];
// 	pid_t	child_process;
// 	while (*temp)
// 	{
// 		if (temp->next->type == PIPE)
// 		pipe(pipefd);
// 		child_process = fork();

// 	}
// }

void	fork_pipes(size_t pipes_nbr, t_tokens **temp, int *fd_in, t_mshell *shell, int *fd_out)
{
	size_t	i;
	// int		last_out;
	int	pipefd[2];
    pid_t	child;
    pid_t	child_processes[pipes_nbr + 1];

    i = 0;
	// last_out = get_last_out(*temp);
    while (i < pipes_nbr)
    {
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
				// out = pipefd[1]
                if (dup2(pipefd[1], 1) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
				close(pipefd[1]);
            }
            close(pipefd[0]);
			// *fd_in = pipefd[0];
			// *fd_out = pipefd[1];
			ft_printf("fd_in = %d, fd_out = %d, temp->content = %s\n\n", *fd_in, *fd_out, (*temp)->content);
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
	// (*temp)->fd_in = pipefd[1];
	ft_printf("FINAL EXEC :fd_in = %d, fd_out = %d, temp->content = %s\n", (*temp)->fd_in, (*temp)->fd_out, (*temp)->content);
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

void	end_pipe(t_mshell *shell, t_tokens *temp)
{
	ft_dprintf(2, "got in end pipe, temp = %s, temp->fd_in = %d, fd_out=%d\n", temp->cmd_arr[0], temp->fd_in, temp->fd_out);
	exec_forwarding(temp, shell);

}

void	middle_pipes(t_mshell *shell, t_tokens *temp)
{
	int fd[2];

	pipe(fd);
	temp->fd_out = fd[1];
	temp->next->next->fd_in = fd[0];
	ft_dprintf(2, "got in MIDDLE pipe, temp = %s, temp->fd_in = %d, fd_out=%d, temp->next->next->in = %d\n", temp->cmd_arr[0], temp->fd_in, temp->fd_out, temp->next->next->fd_in);
	exec_forwarding(temp, shell);
}

void	first_pipe(t_mshell *shell, t_tokens *temp)
{
	int fd[2];

	pipe(fd);
	temp->fd_out = fd[1];
	temp->next->next->fd_in = fd[0];
	ft_dprintf(2, "got in first pipe, temp = %s, temp->fd_in = %d, fd_out=%d, temp->next->next->in = %d\n", temp->cmd_arr[0], temp->fd_in, temp->fd_out, temp->next->next->fd_in);
	exec_forwarding(temp, shell);
}

void	handle_pipes(t_mshell *shell, t_tokens *temp)
{
	size_t	pipes_nbr;
	size_t	i;

	i = 0;
	pipes_nbr = count_successive_pipes(temp);
	ft_dprintf(2, "PIPES NBR = %d\n", pipes_nbr);
	// fork_pipes(pipes_nbr, temp, fd_in, shell, fd_out);
	while (i < pipes_nbr)
	{
		if (i == 0)
		{
			first_pipe(shell, temp);
			temp = temp->next->next;
			ft_dprintf(2, "after first pipe, temp = %s\n", temp->cmd_arr[0]);
		}
		else if (i < pipes_nbr - 1)
		{
			middle_pipes(shell,temp);
			temp = temp->next->next;
			ft_dprintf(2, "after middle pipe, temp = %s\n", temp->cmd_arr[0]);
		}
		else if(i == pipes_nbr)
			end_pipe(shell, temp);
		i++;
	}
	while (wait(NULL) != -1)
	{

	}

	/*1) pour premiere commande : faire un premier pipe = entre cmd0 et cmd 2, pipe[0] pas utilise
		pour les commandes au milieu : creer un autre pipe entre cmdx et cmdy, fd_in = prev_pipe[1] fd_out = new_pipe[1]
		final : fd_in = prev_pipe[0] fd*/
}