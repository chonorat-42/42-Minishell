/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dlist_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:28:31 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/24 17:34:32 by chonorat         ###   ########.fr       */
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
	(*i) = ft_strlen(str);
	while (*i && str[(*i)] != c)
		(*i)--;
}
