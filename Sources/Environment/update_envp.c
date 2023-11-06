/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:31:44 by chonorat          #+#    #+#             */
/*   Updated: 2023/11/06 12:59:56 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_envp_size(t_envp *envp)
{
	int	count;

	count = 0;
	while (envp)
	{
		if (envp->var.readable)
			count++;
		envp = envp->next;
	}
	return (count);
}

static char	*join_varinfo(t_envp *cell)
{
	char	*var;
	char	*join;

	var = ft_strdup(cell->var.name);
	if (!var)
		return (NULL);
	join = ft_strjoin(var, "=");
	if (!join)
		return (free(var), NULL);
	free(var);
	var = join;
	if (cell->var.content)
	{
		join = ft_strjoin(var, cell->var.content);
		if (!join)
			return (free(var), NULL);
		free(var);
		var = join;
	}
	return (var);
}

void	update_envp(t_mshell *shell)
{
	int		index;
	t_envp	*temp;

	if (shell->menvp)
	{
		free_arr(shell->menvp);
		shell->menvp = NULL;
	}
	shell->envp_size = get_envp_size(shell->envp);
	shell->menvp = malloc((shell->envp_size + 1) * sizeof(char *));
	if (!shell->menvp)
		return (free_struct(shell), exit(1));
	shell->menvp[shell->envp_size] = NULL;
	index = 0;
	temp = shell->envp;
	while (temp && index < (int)shell->envp_size)
	{
		if (temp->var.readable)
		{
			shell->menvp[index] = join_varinfo(temp);
			if (!shell->menvp[index++])
				return (free_struct(shell), exit(1));
		}
		temp = temp->next;
	}
}
