/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 01:28:10 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/19 14:05:19 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	mod_shlvl(t_mshell *shell, int shlvl)
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

static void	prepare_shlvl(char *content, int *shlvl)
{
	if (only_digit(content))
	{
		*shlvl = ft_atoi(content);
		(*shlvl)++;
		if (*shlvl < 0)
			*shlvl = 0;
		if (*shlvl >= 1000)
		{
			show_error(NULL, "SHLVL", *shlvl);
			*shlvl = 1;
		}
	}
	else
		*shlvl = 1;
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
		return (free(temp), free_struct(shell), exit(1));
	free(temp);
	prepare_shlvl(content, &shlvl);
	free(content);
	if (!mod_shlvl(shell, shlvl))
		return (free_struct(shell), exit(1));
}
