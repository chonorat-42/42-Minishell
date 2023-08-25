/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:09:39 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:56:30 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp_lst;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		temp_lst = *lst;
		del((*lst)->content);
		*lst = (*lst)->next;
		free(temp_lst);
	}
}
