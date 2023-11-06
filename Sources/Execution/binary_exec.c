/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 17:52:52 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/08 17:52:52 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

void	binary_with_path(t_mshell *shell, char **cmd_arr)
{
	char	*exec;
	char	*temp;
	char	*path;

	exec = get_exec(cmd_arr[0]);
	if (!exec)
		return (free_struct(shell), exit(EXIT_FAILURE));
	path = get_path(cmd_arr[0]);
	if (!path)
		return (free(exec), free_struct(shell), exit(1));
	temp = ft_strjoin(path, exec);
	if (!temp)
		return (free(exec), free_struct(shell),
			free(path), exit(EXIT_FAILURE));
	free(path);
	if (!check_access(temp))
		return (free(exec), free(temp));
	if (execve(temp, cmd_arr, shell->menvp) == -1)
		return (free(exec), free(temp));
}

void	browse_paths(t_mshell *shell, char **cmd_arr)
{
	size_t	j;
	char	*temp;

	j = 0;
	while (shell->paths[j])
	{
		temp = ft_strjoin(shell->paths[j], cmd_arr[0]);
		if (!temp)
			return (free_struct(shell), exit(EXIT_FAILURE));
		if (execve(temp, cmd_arr, shell->menvp) == -1)
			free(temp);
		j++;
	}
	show_error(cmd_arr[0], EXEC, 1);
}

void	bin_exec(t_mshell *shell, char **cmd_arr, int *pid)
{
	if (ft_strchr(cmd_arr[0], '/'))
		binary_with_path(shell, cmd_arr);
	if (!g_status)
	{
		if (shell->paths)
			browse_paths(shell, cmd_arr);
		else
			show_error(cmd_arr[0], EXEC, 2);
	}
	free_struct(shell);
	close_std_fds();
	if (pid)
		free(pid);
	exit(g_status);
}
