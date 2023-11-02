/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlist_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 12:04:11 by chonorat          #+#    #+#             */
/*   Updated: 2023/11/02 14:20:49 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
