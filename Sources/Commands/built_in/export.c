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

static int	add_to_export(t_envp **temp, t_envp *env_cell)
{
	t_envp	*var_cell;

	var_cell = malloc(sizeof(t_envp));
	if (!var_cell)
		return (0);
	var_cell->var.name = ft_strdup(env_cell->var.name);
	if (!var_cell->var.name)
		return (free(var_cell), 0);
	if (env_cell->var.content)
	{
		var_cell->var.content = ft_strdup(env_cell->var.content);
		if (!var_cell->var.content)
			return (free(var_cell->var.name), free(var_cell), 0);
	}
	else
		var_cell->var.content = NULL;
	var_cell->var.alterable = 1;
	var_cell->var.readable = 1;
	if (ft_strncmp(var_cell->var.name, "_", ft_strlen(var_cell->var.name)) == 0)
		var_cell->var.readable = 0;
	if ((*temp)->prev)
		(*temp)->prev->next = var_cell;
	var_cell->prev = (*temp)->prev;
	var_cell->next = *temp;
	(*temp)->prev = var_cell;
	return (1);
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
	if (ft_strncmp(cell->var.name, "_", ft_strlen(cell->var.name)) == 0)
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
				return (free_struct(shell), exit(1));
			break ;
		}
		if (temp->next)
			temp = temp->next;
		else
		{
			if (!add_to_end(&temp, env_cell))
				return (free_struct(shell), exit(1));
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

static int	is_var(char *cmd)
{
	int	index;

	index = 7;
	if (!is_ws(cmd[6]))
		return (0);
	if (!ft_isalpha(cmd[7]) && !is_char_in_set(cmd[index], "_"))
		return (ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", &cmd[7]), 0);
	index++;
	while (cmd[index])
	{
		if (cmd[index] == '=')
			return (1);
		else if (!ft_isalnum(cmd[index]) && !is_char_in_set(cmd[index], "_"))
			return (ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", &cmd[7]), 0);
		index++;
	}
	return (1);
}

static void	add_to_env(t_mshell *shell, t_var *new)
{
	if ((var_exist(shell, new->name) && new->readable == 0) ||
		ft_strncmp(new->name, "_", ft_strlen(new->name)) == 0 ||
		ft_strncmp(new->name, "?", ft_strlen(new->name)) == 0)
	{
		if (new->content)
			free(new->content);
		return (free(new->name));
	}
	if (shell->envp && var_exist(shell, new->name))
		delete_envvar(&shell->envp, new->name, 0);
	create_envp_list(shell, new);
	if (new->content)
		free(new->content);
	free(new->name);
}

static int	split_var(char *var, t_var *new)
{
	int	index;

	index = find_char_index(var, '=');
	if (index > 0)
	{
		new->name = ft_substr(var, 0, index);

		ft_printf("name = %s\n\n", new->name);

		if (!new->name)
			return (0);
		new->content = ft_substr(&var[index + 1], 0, ft_strlen(&var[index + 1]));
		if (!new->content)
			return (free(new->name), 0);
	}
	else
	{
		new->name = ft_strdup(var);
		if (!new->name)
			return (0);
	}
	return (1);
}

static void	get_readable(char *var, t_var *new)
{
	int	index;

	new->name = NULL;
	new->content = NULL;
	new->alterable = 1;
	new->readable = 0;
	index = find_char_index(var, '=');
	if (index > 0)
		new->readable = 1;
}

static void	get_var(t_mshell *shell, char *cmd)
{
	int		start;
	int		end;
	char	*var;
	t_var	new;

	start = 0;
	end = 1;
	var = NULL;
	while (cmd[start] && cmd[start] != '=')
		start++;
	while (!is_ws(cmd[start - 1]))
		start--;
	while (cmd[start + end] && !is_ws(cmd[start + end]))
		end++;
	var = ft_substr(cmd, start, end);
	if (!var)
		return (free_struct(shell), exit(1));
	get_readable(var, &new);
	if (!split_var(var, &new))
		(free_struct(shell), exit(1));
	free(var);
	add_to_env(shell, &new);
}

void	export_case(t_mshell *shell, char *cmd)
{
	char	**var_arr;
	char	*var;
	int		index;

	ft_printf("beginning of export, cmd = %s\n\n", cmd);

	var_arr = ft_split(cmd, ' ');
	var = NULL;
	index = 1;

	ft_printf("var arr :\n");
	print_arr(var_arr);

	while (var_arr[index])
	{
		ft_printf("in export, var_arr[i] = %s\n\n", var_arr[index]);
		var = ft_strjoin("export ", var_arr[index]);
		if (count_arr_size(var_arr) != 1 && is_var(var))
			get_var(shell, var);
		index++;
		free(var);
		var = NULL;
	}
	if (count_arr_size(var_arr) == 1 && ft_strncmp("export", cmd, ft_strlen(cmd)) == 0)
	{
		sort_env(shell);
		print_export(shell->export);
		free_envp(&shell->export);
	}
	free_arr(var_arr);
}