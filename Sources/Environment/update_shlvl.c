/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 01:28:10 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/09 10:27:01 by pgouasmi         ###   ########.fr       */
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
	char	*temp;

	temp = get_envvar_content(shell, shell->envp, "SHLVL");
	if (!temp)
		ft_strdup("0");
	content = ft_strtrim(temp, " \t\n\v\f\r");
	if (!content)
		return (free(temp), free_struct(shell), exit(2));
	free(temp);
	if (only_digit(content))
	{
		shlvl = ft_atoi(content);
		if (shlvl < 0)
			shlvl = 0;
		else
			shlvl++;
	}
	else
		shlvl = 1;
	free(content);
	if (!mod_shlvl(shell, shlvl))
		return (free_struct(shell), exit(2));
}
