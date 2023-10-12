/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:31:44 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/11 17:38:48 by pgouasmi         ###   ########.fr       */
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
	join = ft_strjoin(var, "=");
	free(var);
	var = join;
	if (cell->var.content)
	{
		join = ft_strjoin(var, cell->var.content);
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
		return (free_struct(shell));
	shell->menvp[shell->envp_size] = NULL;
	index = 0;
	temp = shell->envp;
	while (temp && index < (int)shell->envp_size)
	{
		if (temp->var.readable)
			shell->menvp[index++] = join_varinfo(temp);
		temp = temp->next;
	}
}
