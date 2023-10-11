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

void	end_pipe(t_mshell *shell, t_tokens *temp)
{
	if (is_builtin(temp))
		builtin_forwarding(temp, shell);
	else
		executable(temp, shell);
}

void	middle_pipes(t_mshell *shell, t_tokens *temp)
{
	int	fd[2];

	(void)shell;
	pipe(fd);
	temp->fd_out = fd[1];
	temp->next->next->fd_in = fd[0];
	if (is_builtin(temp))
		builtin_forwarding(temp, shell);
	else
		executable(temp, shell);
}

void	first_pipe(t_mshell *shell, t_tokens *temp)
{
	int	fd[2];

	(void)shell;
	if (pipe(fd))
		exit(EXIT_FAILURE);
	temp->fd_out = fd[1];
	temp->next->next->fd_in = fd[0];
	if (is_builtin(temp))
		builtin_forwarding(temp, shell);
	else
		executable(temp, shell);
}

void	handle_pipes(t_mshell *shell, t_tokens *temp)
{
	size_t	pipes_nbr;
	size_t	i;

	i = 0;
	pipes_nbr = count_successive_pipes(temp);
	while (i <= pipes_nbr)
	{
		if (i == 0)
		{
			first_pipe(shell, temp);
			temp = temp->next->next;
		}
		else if (i < pipes_nbr)
		{
			middle_pipes(shell, temp);
			temp = temp->next->next;
		}
		else if (i == pipes_nbr)
			end_pipe(shell, temp);
		i++;
	}
	while (wait(NULL) != -1)
	{
	}
}
