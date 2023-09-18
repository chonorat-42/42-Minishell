/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 12:31:40 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/13 15:33:45 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	print_export(t_envp *export)
{
	while (export)
	{
		if (export->var.readable == 1)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(export->var.name, 1);
			if (export->var.content)
			{
				ft_putstr_fd("=\"", 1);
				ft_putstr_fd(export->var.content, 1);
				ft_putstr_fd("\"", 1);
			}
			ft_putchar_fd('\n', 1);
		}
		export = export->next;
	}
}

void	create_export(t_mshell *shell)
{
	shell->export = malloc(sizeof(t_envp));
	if (!shell->export)
	{
		free_struct(shell);
		exit(1);
	}
	shell->export->var.name = ft_strdup(shell->envp->var.name);
	shell->export->var.content = ft_strdup(shell->envp->var.content);
	if (!shell->export->var.name || !shell->export->var.content)
		return (free_struct(shell), exit(1));
	shell->export->var.readable = 1;
	if (ft_strncmp(shell->export->var.name, "_", ft_strlen(shell->export->var.name)) == 0)
		shell->export->var.readable = 0;
	shell->export->prev = NULL;
	shell->export->next = NULL;
}
