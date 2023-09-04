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

int	delete_envvar(t_envp **envp, char *var)
{
	t_envp	*temp;

	temp = *envp;
	while (temp->next)
	{
		if (!ft_strcmp(var, temp->var_name))
		{
			free(temp->var_cont);
			free(temp->var_name);
			if (temp->prev)
				temp->prev->next = temp->next;
			else
				*envp = temp->next;
			if (temp->next)
				temp->next->prev = temp->prev;
			free(temp);
			return (0);
		}
		temp = temp->next;
	}
	return (1);
}

void	unset_case(t_mshell *shell, char *str)
{
	char	*var;
	char	**to_unset;
	size_t	i;
	
	var = ft_substr(str, 6, ft_strlen(str));
	if (!var)
		return (free_struct(shell), exit (11));
	to_unset = ft_split(var, ' ');
	if (!to_unset)
		return (free(var), free_struct(shell), exit(12));
	i = 0;
	while (to_unset[i])
	{
		if  (!delete_envvar(&shell->envp, to_unset[i]))
		{
			if (!ft_strcmp(to_unset[i], "PATH"))
			{
				free_arr(shell->paths);
				shell->paths = NULL;
			}
		}
		i++;
	}
	free(var);
	free_arr(to_unset);
}
