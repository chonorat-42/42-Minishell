/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:53:31 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 17:07:49 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	t_envp_addback(t_envp **envp, t_envp **new)
{
	t_envp	*temp;

	temp = *envp;
	while (temp->next)
		temp = temp->next;
	(*new)->prev = temp;
	temp->next = *new;
}

void	create_envp_list(t_mshell *shell, t_var *var)
{
	t_envp	*new;

	new = malloc(sizeof(t_envp));
	if (!new)
		return (free_struct(shell), exit(1));
	if (var->content)
	{
		new->var.content = ft_strdup(var->content);
		if (!new->var.content)
			return (free(new), free_struct(shell), exit(1));
	}
	else
		new->var.content = NULL;
	new->var.name = ft_strdup(var->name);
	if (!new->var.name)
		return (free(new), free(new->var.content), free_struct(shell), exit(1));
	new->var.readable = var->readable;
	new->var.alterable = var->alterable;
	new->next = NULL;
	new->prev = NULL;
	if (!shell->envp)
		shell->envp = new;
	else
		t_envp_addback(&shell->envp, &new);
}

void	get_envp(t_mshell *shell, char **envp, char **argv)
{
	size_t	j;
	t_var	new;

	j = 0;
	create_envp(shell, envp, argv);
	while (envp[j])
	{
		new.name = ft_substr(envp[j], 0, find_char_index(envp[j], '='));
		if (!new.name)
			return (free_struct(shell), exit(1));
		new.content = ft_substr(envp[j], find_char_index(envp[j], '=') + 1,
				ft_strlen(envp[j]));
		if (!new.content)
			return (free(new.name), free_struct(shell), exit(1));
		new.readable = 1;
		new.alterable = 1;
		if (ft_strcmp(new.name, "_") == 0)
			new.alterable = 0;
		create_envp_list(shell, &new);
		free(new.name);
		free(new.content);
		j++;
	}
}
