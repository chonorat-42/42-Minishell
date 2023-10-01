/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 17:35:09 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 19:03:11 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	delete_envvar(t_envp **envp, char *var, int ign_param)
{
	t_envp	*temp;

	temp = *envp;
	while (temp)
	{
		if (!ft_strcmp(var, temp->var.name) &&
			(temp->var.alterable || ign_param))
		{
			free(temp->var.content);
			free(temp->var.name);
			if (temp->prev)
				temp->prev->next = temp->next;
			else
				*envp = temp->next;
			if (temp->next)
				temp->next->prev = temp->prev;
			else
				temp->next = NULL;
			free(temp);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

void	unset_case(t_mshell *shell, char **cmd)
{
	int	index;

	index = 1;
	while (cmd[index])
	{
		if (delete_envvar(&shell->envp, cmd[index], 0))
		{
			if (ft_strcmp(cmd[index], "PATH"))
			{
				free_arr(shell->paths);
				shell->paths = NULL;
			}
		}
		index++;
	}
}
