/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_svar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:14:40 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/11 17:39:37 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	get_svar(t_mshell *shell)
{
	t_var	status;
	char	*content;

	content = ft_itoa(g_status);
	status.name = ft_strdup("?");
	status.content = ft_strdup(content);
	status.readable = 0;
	status.alterable = 0;
	if (shell->envp && var_exist(shell, "?"))
		delete_envvar(&shell->envp, "?", 1);
	create_envp_list(shell, &status);
	free(status.name);
	free(status.content);
	free(content);
	shell->exit_status = g_status;
	g_status = 0;
}
