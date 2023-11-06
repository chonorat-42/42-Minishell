/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dlst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:50:15 by chonorat          #+#    #+#             */
/*   Updated: 2023/11/06 13:42:56 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_dlist(t_mshell *shell, t_envp *envp, t_dlist **lst)
{
	t_dlist	*temp;
	char	*expanded;

	temp = *lst;
	while (temp)
	{
		if (find_char_index(temp->content, '$') >= 0)
		{
			expanded = expand_envvar(shell, temp->content, envp);
			free(temp->content);
			temp->content = expanded;
		}
		temp = temp->next;
	}
}

void	has_expansion(t_mshell *shell, t_dlist **lst, t_dlist *temp,
	t_envp *envp)
{
	char	*name;
	char	*content;

	name = ft_substr(temp->content, 1, ft_strlen(temp->content));
	if (!name)
		return (free_struct(shell), free_dlist(lst), exit(1));
	content = get_envvar_content(shell, envp, name);
	multifree(name, temp->content, 0, 0);
	if (content)
	{
		temp->content = ft_strdup(content);
		free(content);
	}
	else
		temp->content = ft_strdup("");
}

void	expand_dlst(t_mshell *shell, t_dlist **lst, t_envp *envp)
{
	t_dlist	*temp;

	temp = *lst;
	while (temp)
	{
		if (temp->content[0] == '$')
		{
			if (temp->content[1])
			{
				if (temp->content[1] != '$')
					has_expansion(shell, lst, temp, envp);
			}
			// else
			// 	temp->content = ft_strdup("");
		}
		temp = temp->next;
	}
}
