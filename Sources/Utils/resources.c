/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resources.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:34:33 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/17 16:50:41 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	move_to_next_quote(char *str, size_t *i, char c)
{
	(*i)++;
	while (str[(*i)] && str[(*i)] != c)
		(*i)++;
}

void	print_arr(char **arr)
{
	int j;

	if (DEBUG)
	{
		j = -1;
		while(arr[++j])
			printf("%s\n", arr[j]);
	}
}
