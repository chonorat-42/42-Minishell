/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adress_keeper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 12:51:28 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/22 14:27:05 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_mshell	*adress_keeper(t_mshell *shell)
{
	static t_mshell	*shell_adr;

	if (!shell)
		return (shell_adr);
	shell_adr = shell;
	return (shell_adr);
}

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
