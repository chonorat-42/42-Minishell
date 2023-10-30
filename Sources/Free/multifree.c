/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multifree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:02:20 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/27 16:38:26 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rm_hdoc_child(t_mshell *shell)
{
	char	**cmd;
	char	*join;
	size_t	j;

	j = 0;
	cmd = ft_split("rm -rf /tmp/temp.heredoc2", ' ');
	if (!cmd)
		return (free_struct(shell), exit(1));
	while (shell->paths[j])
	{
		join = ft_strjoin(shell->paths[j], cmd[0]);
		if (!join)
			return (free_struct(shell), free_arr(shell->paths), exit(1));
		if (execve(join, cmd, shell->menvp) == -1)
			free(join);
		j++;
	}
	return (free_struct(shell), free_arr(cmd), exit(0));
}

void	remove_hdoc(t_mshell *shell)
{
	pid_t	child;

	if (access("/tmp/temp.heredoc2", F_OK) != -1 && shell->paths)
	{
		child = fork();
		if (child == -1)
			return (free_struct(shell), exit(1));
		if (!child)
			rm_hdoc_child(shell);
		else
			waitpid(child, NULL, 0);
	}
}

void	multifree(void *ptr1, void *ptr2, void *ptr3, void *ptr4)
{
	if (ptr1)
	{
		free(ptr1);
		ptr1 = NULL;
	}
	if (ptr2)
	{
		free(ptr2);
		ptr2 = NULL;
	}
	if (ptr3)
	{
		free(ptr3);
		ptr3 = NULL;
	}
	if (ptr4)
	{
		free(ptr4);
		ptr4 = NULL;
	}
}

void	free_errors(t_error *error)
{
	t_error	*temp;

	while (error)
	{
		if (error->content)
			free(error->content);
		temp = error;
		error = temp->next;
		free(temp);
	}
	error = NULL;
}
