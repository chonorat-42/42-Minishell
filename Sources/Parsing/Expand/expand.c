/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:26:35 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 18:06:50 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_expand_tab(size_t index[2], size_t qcount[2])
{
	index[0] = 0;
	index[1] = 0;
	qcount[0] = 0;
	qcount[1] = 0;
}

void	split_envvar(char *str, t_dlist **lst)
{
	size_t	index[2];
	size_t	qcount[2];

	init_expand_tab(index, qcount);
	while (str[index[0]])
	{
		if (str[index[0]] == '\'' && !qcount[1] % 2)
			qcount[0]++;
		if (str[index[0]] == '\'')
			index[0]++;
		else if (str[index[0]] == '\"')
		{
			if (!qcount[0] % 2)
				qcount[1]++;
			index[0]++;
		}
		else if (str[index[0]] == '$')
			envvar_flag(index, qcount, lst, str);
		else
			index[0]++;
	}
	if (index[1] != index[0])
		split_into_dlst(lst, str, index[0], index[1]);
}

char	*expand_envvar(t_mshell *shell, char *str, t_envp *envp)
{
	char	*result;
	t_dlist	*temp;

	temp = NULL;
	split_envvar(str, &temp);
	expand_dlst(shell, &temp, envp);
	result = join_dlist(temp);
	free_dlist(&temp);
	return (result);
}

int	expand(t_mshell *shell, char *cmd)
{
	char	*temp;

	temp = NULL;
	if ((ft_char_index(cmd, '$') >= 0))
	{
		temp = expand_envvar(shell, cmd, shell->envp);
		if (!temp)
			return (1);
		free(shell->input);
		if (temp)
		{
			shell->input = ft_strdup(temp);
			if (!shell->input)
				return (free(temp), 1);
		}
		else
		{
			shell->input = ft_strdup("");
			if (!shell->input)
				return (free(temp), 1);
		}
		free(temp);
	}
	return (0);
}
