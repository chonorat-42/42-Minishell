/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multifree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:02:20 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/24 12:08:41 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	multifree(void *ptr1, void *ptr2, void *ptr3, void *ptr4)
{
	if (ptr1)
	{
		free(ptr1);
		ptr1 = NULL;
	}
	if (ptr2)
	{
		free(ptr2);
		ptr2 = NULL;
	}
	if (ptr3)
	{
		free(ptr3);
		ptr3 = NULL;
	}
	if (ptr4)
	{
		free(ptr4);
		ptr4 = NULL;
	}
}

void	free_errors(t_error *error)
{
	t_error	*temp;

	while (error)
	{
		if (error->content)
			free(error->content);
		temp = error;
		error = temp->next;
		free(temp);
	}
	error = NULL;
}
