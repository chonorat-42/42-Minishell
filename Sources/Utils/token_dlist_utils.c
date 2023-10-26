/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dlist_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:28:31 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/25 16:19:55 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	size_t	j;
	size_t	q_count;

	j = *i;
	q_count = 1;
	(*i)++;
	while (str[(*i)])
	{
		if (str[(*i)] == c)
			q_count++;
		if (str[*i + 1] && (ft_isws(str[*i + 1]) || is_char_in_set(str[(*i + 1)], "<>")) && !(q_count % 2))
		{
			if (!is_char_in_set(str[(*i + 1)], "<>") && !ft_isws(str[(*i + 1)]))
				(*i)++;
			return ;
		}
		(*i)++;
	}
}
