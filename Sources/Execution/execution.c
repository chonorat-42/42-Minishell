/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:48:14 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 19:06:58 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

void	executable(t_tokens *temp, t_mshell *shell)
{
	pid_t	child;

	child = fork();
	if (child == -1)
		return (free_struct(shell), exit(EXIT_FAILURE));
	if (!child)
	{
		exec_sig(shell);
		manage_fd(temp->fd_in, temp->fd_out);
		temp->fd_in = 0;
		temp->fd_out = 1;
		bin_exec(shell, temp->cmd_arr);
	}
	else
	{
		if (temp->fd_in != STDIN_FILENO)
			close(temp->fd_in);
		if (temp->fd_out != STDOUT_FILENO)
			close(temp->fd_out);
		if (waitpid(child, (int *)&g_status, 0) == -1)
			return (perror("waitpid"), free_struct(shell), exit(EXIT_FAILURE));
	}
	if (temp->cmd_arr)
		free_arr(temp->cmd_arr);
	return (temp->cmd_arr = NULL, get_fork_status());
}

void	exec_forwarding(t_tokens *temp, t_mshell *shell)
{
	if (has_bad_fd(temp))
	{
		g_status = 1;
		return (print_errors(temp));
	}
	if (is_builtin(temp))
		builtin_forwarding(temp, shell);
	else
	{
		ignore_sig(shell);
		executable(temp, shell);
	}
}

void	execution(t_mshell *shell)
{
	t_tokens	*temp;

	// ft_dprintf(2, "got in execution\n");
	temp = shell->tok_lst;
	if (temp->next && temp->next->type == PIPE)
		handle_pipes(shell, temp);
	else
		exec_forwarding(temp, shell);
	free(shell->input);
	free_tokens(&shell->tok_lst);
}
