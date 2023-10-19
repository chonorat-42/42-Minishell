/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_svar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:14:40 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/19 16:30:08 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

void	get_svar(t_mshell *shell)
{
	t_var	status;
	char	*content;

	content = ft_itoa(g_status);
	if (!content)
		return (free_struct(shell), exit(1));
	status.name = ft_strdup("?");
	if (!status.name)
		return (free(content), free_struct(shell), exit(1));
	status.content = ft_strdup(content);
	if (!status.content)
		return (free(content), free(status.name), free_struct(shell), exit(1));
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
