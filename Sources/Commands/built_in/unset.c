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

static int	check_var(char *cmd, char *arg)
{
	int	index;

	if (!ft_isalpha(arg[0]) && !is_char_in_set(arg[0], "_"))
		return (builtin_error(cmd, arg, 1), 0);
	index = 1;
	while (arg[index])
	{
		if (!ft_isalnum(arg[index]) && !is_char_in_set(arg[index], "_"))
			return (builtin_error(cmd, arg, 1), 0);
		index++;
	}
	return (1);
}

void	unset_case(t_mshell *shell, char **cmd)
{
	int	index;

	index = 1;
	while (cmd[index])
	{
		if (check_var(cmd[0], cmd[index]))
		{
			if (delete_envvar(&shell->envp, cmd[index], 0))
			{
				if (ft_strncmp(cmd[index], "PATH", ft_strlen(cmd[index])) == 0)
				{
					free_arr(shell->paths);
					shell->paths = NULL;
				}
			}
		}
		index++;
	}
}
