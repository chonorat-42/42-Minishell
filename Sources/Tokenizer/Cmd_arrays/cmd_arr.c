/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:24:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/06 19:24:51 by pgouasmi         ###   ########.fr       */
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

char	**list_into_arr(t_dlist *lst)
{
	char	**res;
	t_dlist	*temp;
	size_t	size;
	size_t	j;

	res = NULL;
	temp = lst;
	size = dlst_size(lst);
	res = malloc(sizeof(char *) * (size + 1));
	res[size] = NULL;
	j = 0;
	while (j < size)
	{
		res[j] = ft_strdup(temp->content);
		j++;
		temp = temp->next;
	}
	return (res);
}

void	get_commands_lst(t_dlist *base, t_dlist **new)
{
	t_dlist	*temp;

	temp = base;
	while (temp)
	{
		if (is_char_in_set(temp->content[0], "<>"))
			temp = temp->next->next;
		else
		{
			split_into_dlst(new, temp->content, ft_strlen(temp->content), 0);
			temp = temp->next;
		}
	}
}

void	create_cmd_arr(t_tokens **tk_lst, t_mshell *shell)
{
	t_tokens	*temp;
	t_dlist		*new;

		temp = *tk_lst;
		while (temp)
		{
			new = NULL;
			get_commands_lst(temp->dlst, &new);
			if (!new)
				return (free_arr(shell->paths), free(shell->input),
					ft_free_tokens(tk_lst), get_input_loop(shell));
			temp->cmd_arr = list_into_arr(new);
			if (!temp->cmd_arr)
				return (free_arr(shell->paths), free(shell->input),
					ft_free_tokens(tk_lst), get_input_loop(shell));
			free_dlist(&temp->dlst);
			free_dlist(&new);
			temp = temp->next;
		}
}