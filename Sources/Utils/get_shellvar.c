/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_shellvar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:14:40 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/14 18:18:57 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_status;

void	get_shellvar(t_mshell *shell)
{
	t_var	status;

	status.name = ft_strdup("?");
	status.content = ft_strdup(ft_itoa(g_status));
	status.readable = 0;
	status.alterable = 0;
	if (shell->envp && var_exist(shell, "?"))
		delete_envvar(&shell->envp, "?", 1);
	create_envp_list(shell, &status);
	free(status.name);
	free(status.content);
}