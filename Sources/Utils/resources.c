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

/*void	print_lst(t_list *lst)
{
		t_list	*temp;

	temp = lst;
	while (temp)
	{
		ft_printf("%s\n", temp->cmd);
		temp = temp->next;
	}
}*/

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

size_t	count_arr_size(char **arr)
{
	size_t i;

	i = 0;
	if (!arr)
		return (0);
	while(arr[i])
		i++;
	return (i);
}