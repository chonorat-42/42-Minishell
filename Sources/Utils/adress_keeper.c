/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adress_keeper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 12:51:28 by chonorat          #+#    #+#             */
/*   Updated: 2023/11/02 13:32:52 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*adress_keeper(void *adress)
{
	static void	*adr;

	if (!adress)
		return (adr);
	adr = adress;
	return (adr);
}

t_mshell	*shell_keeper(t_mshell *shell)
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
