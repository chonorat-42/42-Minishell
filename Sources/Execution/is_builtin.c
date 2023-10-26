/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:22:33 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/26 14:34:43 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long g_status;

int	is_builtin(t_tokens *temp)
{
	if (!ft_strcmp(temp->cmd_arr[0], "echo")
		|| !ft_strcmp(temp->cmd_arr[0], "cd")
		|| !ft_strcmp(temp->cmd_arr[0], "exit")
		|| !ft_strcmp(temp->cmd_arr[0], "env")
		|| !ft_strcmp(temp->cmd_arr[0], "unset")
		|| !ft_strcmp(temp->cmd_arr[0], "pwd")
		|| !ft_strcmp(temp->cmd_arr[0], "export"))
		return (g_status = 0, 1);
	return (0);
}

void	builtin_forwarding_pipe(t_tokens *temp, t_mshell *shell)
{
	ft_dprintf(2, "temp = %s, has bad fd = %d\n", temp->content, temp->has_bad_fd);
	if (!temp->has_bad_fd)
	{
		if (!ft_strcmp(temp->cmd_arr[0], "echo"))
			echo_case(temp->cmd_arr, 1);
		else if (!ft_strcmp(temp->cmd_arr[0], "cd"))
			cd_case(shell, temp->cmd_arr);
		else if (!ft_strcmp(temp->cmd_arr[0], "exit"))
			exit_case(shell, temp->cmd_arr);
		else if (!ft_strcmp(temp->cmd_arr[0], "env"))
			env_case(shell, temp->cmd_arr, 1);
		else if (!ft_strcmp(temp->cmd_arr[0], "unset"))
			unset_case(shell, temp->cmd_arr);
		else if (!ft_strcmp(temp->cmd_arr[0], "pwd"))
			pwd_case(shell, temp->cmd_arr, 1);
		else if (!ft_strcmp(temp->cmd_arr[0], "export"))
			export_case(shell, temp->cmd_arr, 1);
	}
	else
		g_status = 1;
}

void	builtin_forwarding(t_tokens *temp, t_mshell *shell)
{
	if (!ft_strcmp(temp->cmd_arr[0], "echo"))
		echo_case(temp->cmd_arr, temp->fd_out);
	else if (!ft_strcmp(temp->cmd_arr[0], "cd"))
		cd_case(shell, temp->cmd_arr);
	else if (!ft_strcmp(temp->cmd_arr[0], "exit"))
		exit_case(shell, temp->cmd_arr);
	else if (!ft_strcmp(temp->cmd_arr[0], "env"))
		env_case(shell, temp->cmd_arr, temp->fd_out);
	else if (!ft_strcmp(temp->cmd_arr[0], "unset"))
		unset_case(shell, temp->cmd_arr);
	else if (!ft_strcmp(temp->cmd_arr[0], "pwd"))
		pwd_case(shell, temp->cmd_arr, temp->fd_out);
	else if (!ft_strcmp(temp->cmd_arr[0], "export"))
		export_case(shell, temp->cmd_arr, temp->fd_out);
}
