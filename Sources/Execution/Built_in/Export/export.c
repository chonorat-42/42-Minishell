/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:08:23 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/13 15:23:58 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static int	add_to_export(t_envp **temp, t_envp *env_cell)
{
	t_envp	*var_cell;

	var_cell = malloc(sizeof(t_envp));
	if (!var_cell)
		return (0);
	var_cell->var.name = ft_strdup(env_cell->var.name);
	if (!var_cell->var.name)
		return (free(var_cell), 0);
	var_cell->var.content = NULL;
	if (env_cell->var.content)
	{
		var_cell->var.content = ft_strdup(env_cell->var.content);
		if (!var_cell->var.content)
			return (free(var_cell->var.name), free(var_cell), 0);
	}
	var_cell->var.alterable = 1;
	var_cell->var.readable = 1;
	if (ft_strncmp(var_cell->var.name, "_", ft_strlen(var_cell->var.name)) == 0
		|| ft_strncmp(var_cell->var.name, "?",
			ft_strlen(var_cell->var.name)) == 0)
		var_cell->var.readable = 0;
	if ((*temp)->prev)
		(*temp)->prev->next = var_cell;
	var_cell->prev = (*temp)->prev;
	return (var_cell->next = *temp, (*temp)->prev = var_cell, 1);
}

static int	add_to_end(t_envp **temp, t_envp *env_cell)
{
	t_envp	*cell;

	cell = malloc(sizeof(t_envp));
	if (!cell)
		return (0);
	cell->var.name = ft_strdup(env_cell->var.name);
	if (!cell->var.name)
		return (free(cell), 0);
	cell->var.content = NULL;
	if (env_cell->var.content)
	{
		cell->var.content = ft_strdup(env_cell->var.content);
		if (!cell->var.content)
			return (free(cell->var.name), free(cell), 0);
	}
	cell->var.alterable = 1;
	cell->var.readable = 1;
	if (ft_strncmp(cell->var.name, "_", ft_strlen(cell->var.name)) == 0
		|| ft_strncmp(cell->var.name, "?", ft_strlen(cell->var.name)) == 0)
		cell->var.readable = 0;
	(*temp)->next = cell;
	cell->next = NULL;
	cell->prev = *temp;
	return (1);
}

static void	get_pos(t_mshell *shell, t_envp *env_cell)
{
	t_envp	*temp;

	temp = shell->export;
	while (temp)
	{
		if (ft_strcmp(temp->var.name, env_cell->var.name) > 0)
		{
			if (!add_to_export(&temp, env_cell))
				return (free_struct(shell), exit(EXIT_FAILURE));
			break ;
		}
		if (temp->next)
			temp = temp->next;
		else
		{
			if (!add_to_end(&temp, env_cell))
				return (free_struct(shell), exit(EXIT_FAILURE));
			temp = NULL;
		}
	}
	while (shell->export->prev)
		shell->export = shell->export->prev;
}

static void	sort_env(t_mshell *shell)
{
	t_envp	*temp;

	if (shell->envp)
		create_export(&shell->export, shell->envp);
	else
		return ;
	temp = shell->envp->next;
	while (temp)
	{
		get_pos(shell, temp);
		temp = temp->next;
	}
}

void	export_case(t_mshell *shell, char **cmd, int fd)
{
	int	index;

	index = 1;
	while (cmd[index])
	{
		if (check_option(shell, cmd))
			break ;
		if (is_var(cmd[index]))
			get_var(shell, cmd[index]);
		index++;
	}
	if (ft_arr_size(cmd) == 1)
	{
		sort_env(shell);
		print_export(shell->export, fd);
		free_envp(&shell->export);
	}
}
