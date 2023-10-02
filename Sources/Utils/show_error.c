/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:29 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/02 13:01:18 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_error(char *cmd, int error)
{
	if (error == 1)
		ft_dprintf(2, "%s: command not found\n", cmd);
	else if (error == 2)
		ft_dprintf(2, "%s: No such file or directory\n", cmd);
}

void	show_error(char *cmd, char *type, int error)
{
	ft_putstr_fd("minishell: ", 2);
	if (ft_strcmp(type, "EXEC") == 0)
		exec_error(cmd, error);
	if (ft_strcmp(type, "QUOTES") == 0)
		ft_putendl_fd("quotes unclosed", 2);
}