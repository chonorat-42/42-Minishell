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

int	var_exist(t_mshell *shell, char *var)
{
	t_envp	*temp;

	temp = shell->envp;
	while (temp)
	{
		if (ft_strcmp(var, temp->var.name) == 0)
			return (1);
		temp = temp->next;
	}
	return (0);
}

void	print_export(t_envp *export, int fd)
{
	while (export)
	{
		if (export->var.readable == 1)
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(export->var.name, fd);
			if (export->var.content)
			{
				ft_putstr_fd("=\"", fd);
				ft_putstr_fd(export->var.content, fd);
				ft_putstr_fd("\"", fd);
			}
			ft_putchar_fd('\n', fd);
		}
		export = export->next;
	}
}

int	create_export(t_envp **export, t_envp *envp)
{
	*export = malloc(sizeof(t_envp));
	if (!*export)
		return (0);
	(*export)->var.name = ft_strdup(envp->var.name);
	(*export)->var.content = ft_strdup(envp->var.content);
	if (!(*export)->var.name || !(*export)->var.content)
		return (0);
	(*export)->var.readable = 1;
	(*export)->var.alterable = 1;
	if (ft_strncmp((*export)->var.name, "_", ft_strlen((*export)->var.name)) == 0 ||
			ft_strncmp((*export)->var.name, "?", ft_strlen((*export)->var.name)) == 0)
	{
		(*export)->var.readable = 0;
		(*export)->var.alterable = 0;
	}
	(*export)->prev = NULL;
	(*export)->next = NULL;
	return (1);
}
