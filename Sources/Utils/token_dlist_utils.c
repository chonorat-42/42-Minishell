/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dlist_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:28:31 by chonorat          #+#    #+#             */
/*   Updated: 2023/11/02 14:06:25 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	last_quote_redir(char *str, t_dlist **lst, size_t *i, size_t *j)
{
	while (is_char_in_set(str[(*i)], "\'\""))
	{
		move_to_last_quote(str, i, str[*j]);
		if ((!str[(*i)] || !str[(*i + 1)] || ft_isws(str[(*i + 1)])
				|| is_char_in_set(str[(*i + 1)], "<>")))
		{
			split_into_dlst(lst, str, ++(*i), *j);
			return ;
		}
		else
		{
			if (str[(*i)])
				(*i)++;
		}
	}
}

int	split_into_dlst(t_dlist **lst, char *str, size_t i, size_t j)
{
	t_dlist	*new;
	t_dlist	*temp;

	new = malloc(sizeof(t_dlist));
	if (!new)
		return (1);
	new->next = NULL;
	new->content = ft_substr(str, j, i - j);
	if (!new->content)
		return (free(new), 1);
	if (!*lst)
	{
		*lst = new;
		new->prev = NULL;
	}
	else
	{
		temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
		new->prev = temp;
	}
	return (0);
}

void	get_chevrons(char *str, size_t *i, char c, t_dlist **lst)
{
	size_t	j;

	j = *i;
	while (str[(*i)] == c)
		(*i)++;
	split_into_dlst(lst, str, *i, j);
}

void	move_to_last_quote(char *str, size_t *i, char c)
{
	size_t	q_count;

	q_count = 1;
	(*i)++;
	while (str[(*i)])
	{
		if (str[(*i)] == c)
			q_count++;
		if ((!str[*i + 1]
				|| (ft_isws(str[*i + 1])
					|| is_char_in_set(str[(*i + 1)], "<>")))
			&& !(q_count % 2))
			return ;
		(*i)++;
	}
}
