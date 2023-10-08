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

char *get_exec(char *cmd)
{
	size_t	j;
	char	*res;

	j = 0;
	while (cmd[j])
		j++;
	while (j != 0 && cmd[j] != '/')
		j--;
	j++;
	res = ft_substr(cmd, j, ft_strlen(cmd));
	return (res);
}

char	*get_path(char *cmd)
{
	size_t	i;
	char	*res;

	i = 0;
	while (cmd[i])
		i++;
	while (cmd[i] != '/')
		i--;
	i++;
	res = ft_substr(cmd, 0, i);
	return (res);
}

void	manage_fd(int fd_in, int fd_out)
{
	if (fd_out != 1)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (fd_in != 0)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
}

void	binary_with_path(t_mshell *shell, char **cmd_arr)
{
	char	*exec;
	char	**exec_split;
	char	*temp;

	exec = get_exec(cmd_arr[0]);
	if (!exec)
		return (free_struct(shell), exit(1));
	exec_split = ft_split(exec, ' ');
	if (!exec_split)
		return (free(exec), free_struct(shell), exit(2));
	temp = ft_strjoin(get_path(cmd_arr[0]), exec);
	if (!temp)
		return (free(exec), free_arr(exec_split), free_struct(shell), exit(3));
	if (execve(temp, exec_split, shell->menvp) == -1)
	{
		free(exec);
		free_arr(exec_split);
		free(temp);
	}
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
			return (free_struct(shell), exit(1));
		if (execve(temp, cmd_arr, shell->menvp) == -1)
			free(temp);
		j++;
	}
	show_error(cmd_arr[0], "EXEC", 1);
}

void	bin_exec(t_mshell *shell, char **cmd_arr, int fd_in, int fd_out)
{
	manage_fd(fd_in, fd_out);
	if (ft_strchr(cmd_arr[0], '/'))
		binary_with_path(shell, cmd_arr);
	if (shell->paths)
		browse_paths(shell, cmd_arr);
	else
		show_error(cmd_arr[0], "EXEC", 2);
	free_struct(shell);
	if (fd_out != 1)
		close(fd_out);
	if (fd_in != 0)
		close(fd_in);
	exit(127);
}
