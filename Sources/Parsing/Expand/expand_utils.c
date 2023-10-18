/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:03:32 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/06 18:03:32 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_char_in_set(char c, char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

size_t	last_envvar_char(char *str)
{
	size_t	i;

	i = 1;

	ft_dprintf(2, "got in last envvar char\n");

	while (str[i])
	{
		if (ft_isws(str[i]) || is_char_in_set(str[i], "\"\'=$"))
			return (i);
		i++;
	}
	return (i);
}
