/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adress_keeper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 12:51:28 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/17 12:52:23 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*fd_keeper(int *fd)
{
	static int	*fd_adress;

	if (fd == NULL)
		return (fd_adress);
	fd_adress = fd;
	return (fd);
}

t_dlist	**dlist_keeper(t_dlist **lst)
{
	static t_dlist	**lst_adress;

	if (lst == NULL)
		return (lst_adress);
	lst_adress = lst;
	return (lst);
}