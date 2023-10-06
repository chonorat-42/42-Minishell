/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 01:28:10 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/06 13:01:06 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int mod_shlvl(t_mshell *shell, int shlvl)
{
	char	*lvl;
	t_envp	*temp;

	lvl = ft_itoa(shlvl);
	if (!lvl)
		return (0);
	temp = shell->envp;
	while (temp)
	{
		if (ft_strcmp(temp->var.name, "SHLVL") == 0)
		{
			if (temp->var.content)
				free(temp->var.content);
			temp->var.readable = 1;
			temp->var.content = lvl;
			break ;
		}
		temp = temp->next;
	}
	return (1);
}

void	update_shlvl(t_mshell *shell)
{
	int		shlvl;
	char	*content;

	content = get_envvar_content(shell->envp, "SHLVL");
	if (!content)
		return (free_struct(shell), exit(2));
	if (content[0] && only_digit(content))
	{
		shlvl = ft_atoi(content);
		shlvl++;
	}
	else
		shlvl = 1;
	free(content);
	if (!mod_shlvl(shell, shlvl))
		return (free_struct(shell), exit(2));
}