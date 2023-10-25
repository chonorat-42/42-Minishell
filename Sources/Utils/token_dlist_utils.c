/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dlist_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:28:31 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/25 22:02:23 by chonorat         ###   ########.fr       */
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
	size_t	q_count;

	q_count = 1;
	(*i)++;
	while (str[(*i)])
	{
		if (str[(*i)] == c)
			q_count++;
		if (str[*i + 1] && (ft_isws(str[*i + 1])
				|| is_char_in_set(str[(*i + 1)], "<>")) && !(q_count % 2))
		{
			if (!is_char_in_set(str[(*i + 1)], "<>") && !ft_isws(str[(*i + 1)]))
				(*i)++;
			return ;
		}
		(*i)++;
	}
}

void	move_to_last_quote2(char *str, size_t *i, char c)
{
	(*i) = ft_strlen(str);
	while (*i && str[(*i)] != c)
		(*i)--;
}
