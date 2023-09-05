/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 12:31:40 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/04 16:49:41 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	print_export(t_export *export)
{
	while (export)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(export->var, 1);
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(export->content, 1);
		ft_putstr_fd("\"", 1);
		ft_putchar_fd('\n', 1);
		export = export->next;
	}
}

void	create_export(t_mshell *shell)
{
	shell->export = malloc(sizeof(t_export));
	if (!shell->export)
	{
		free_struct(shell);
		exit(1);
	}
	shell->export->var = ft_strdup(shell->envp->var_name);
	shell->export->content = ft_strdup(shell->envp->var_cont);
	if (!shell->export->var || !shell->export->content)
	{
		free_struct(shell);
		exit(1);
	}
	shell->export->prev = NULL;
	shell->export->next = NULL;
}
