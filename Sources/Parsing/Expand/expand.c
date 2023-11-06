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

int	is_between_simple(char *str)
{
	size_t	i;
	size_t	s_quote_count;

	i = 0;
	s_quote_count = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			move_to_last_quote(str, &i, str[i]);
		if (str[i] == '\'')
			s_quote_count++;
		if ((str[i]) == '$')
			return (s_quote_count % 2);
		i++;
	}
	return (0);
}

void	split_envvar(char *str, t_dlist **lst)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && !is_between_simple(str))
		{
			if (j != i)
				split_into_dlst(lst, str, i, j);
			j = i;
			i += last_envvar_char(&str[i]);
			split_into_dlst(lst, str, i, j);
			j = i;
		}
		else
			i++;
	}
	if (j != i)
		split_into_dlst(lst, str, i, j);
}

char	*join_dlist(t_dlist	*lst)
{
	char	*res;
	char	*temp;
	char	*join;
	t_dlist	*temp_lst;

	res = NULL;
	temp_lst = lst;
	while (temp_lst)
	{
		if (temp_lst->content && temp_lst->content[0])
		{
			temp = ft_strdup(temp_lst->content);
			if (!res)
				res = ft_strdup(temp);
			else
			{
				join = ft_strjoin(res, temp);
				free(res);
				res = join;
			}
			free(temp);
		}
		temp_lst = temp_lst->next;
	}
	return (res);
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
