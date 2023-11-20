/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlist_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 12:04:11 by chonorat          #+#    #+#             */
/*   Updated: 2023/11/10 16:18:05 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hd_delim_quotes(t_mshell *shell, char **new_del, char **delim,
	int *del_quote)
{
	*del_quote = 1;
	*new_del = remove_quotes(*delim);
	free(*delim);
	*delim = ft_strdup(*new_del);
	if (!*delim)
		return (free_struct(shell), free(*new_del), exit(1));
	free(*new_del);
}

static int	get_temp_lst(char **temp, char **join, char **res,
	t_dlist *temp_lst)
{
	if (temp_lst->content && temp_lst->content[0])
	{
		*temp = ft_strdup(temp_lst->content);
		if (!*temp)
			return (0);
		if (!*res)
		{
			*res = ft_strdup(*temp);
			if (!*res)
				return (free(*temp), 0);
		}
		else
		{
			*join = ft_strjoin(*res, *temp);
			if (!*join)
				return (free(*res), free(*temp), free_dlist(&temp_lst), 0);
			free(*res);
			*res = *join;
		}
		free(*temp);
	}
	return (1);
}

char	*join_dlist(t_dlist *lst)
{
	char	*res;
	char	*temp;
	char	*join;
	t_dlist	*temp_lst;

	res = NULL;
	temp_lst = lst;
	while (temp_lst)
	{
		if (!get_temp_lst(&temp, &join, &res, temp_lst))
			return (free_dlist(&lst), exit(1), NULL);
		temp_lst = temp_lst->next;
	}
	return (res);
}

size_t	dlst_size(t_dlist *lst)
{
	size_t	res;
	t_dlist	*temp;

	res = 0;
	temp = lst;
	while (temp)
	{
		res++;
		temp = temp->next;
	}
	return (res);
}

char	**list_into_arr(t_mshell *shell, t_dlist *lst)
{
	char	**res;
	t_dlist	*temp;
	size_t	size;
	size_t	j;

	if (!lst)
		return (NULL);
	res = NULL;
	temp = lst;
	size = dlst_size(lst);
	res = malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (free_struct(shell), exit(1), NULL);
	res[size] = NULL;
	j = 0;
	while (j < size)
	{
		res[j] = ft_strdup(temp->content);
		if (!res[j])
			return (free_struct(shell), exit(1), NULL);
		j++;
		temp = temp->next;
	}
	return (res);
}
