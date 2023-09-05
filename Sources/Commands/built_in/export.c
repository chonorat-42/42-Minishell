/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:08:23 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/05 11:58:10 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

static int	add_to_export(t_export **temp, t_envp *env_cell)
{
	t_export	*var_cell;

	var_cell = malloc(sizeof(t_export));
	if (!var_cell)
		return (0);
	var_cell->var = ft_strdup(env_cell->var_name);
	if (!var_cell->var)
		return (free(var_cell), 0);
	var_cell->content = ft_strdup(env_cell->var_cont);
	if (!var_cell->content)
		return (free(var_cell->var), free(var_cell), 0);
	if ((*temp)->prev)
		(*temp)->prev->next = var_cell;
	var_cell->prev = (*temp)->prev;
	var_cell->next = *temp;
	(*temp)->prev = var_cell;
	return (1);
}

static int	add_to_end(t_export **temp, t_envp *env_cell)
{
	t_export	*cell;

	cell = malloc(sizeof(t_export));
	if (!cell)
		return (0);
	cell->var = ft_strdup(env_cell->var_name);
	if (!cell->var)
		return (free(cell), 0);
	cell->content = ft_strdup(env_cell->var_cont);
	if (!cell->content)
		return (free(cell->var), free(cell), 0);
	(*temp)->next = cell;
	cell->next = NULL;
	cell->prev = *temp;
	return (1);
}

static void	get_pos(t_mshell *shell, t_envp *env_cell)
{
	t_export	*temp;

	temp = shell->export;
	while (temp)
	{
		if (ft_strcmp(temp->var, env_cell->var_name) > 0)
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

	temp = shell->envp;
	if (shell->envp)
		create_export(shell);
	while (temp)
	{
		get_pos(shell, temp);
		temp = temp->next;
	}
}

static int	is_var(char *cmd)
{
	int	index;
	int	tmp_index;
	int	equal_sign;

	index = 0;
	equal_sign = 0;
	tmp_index = 0;
	if (!is_ws(cmd[6]))
		return (0);
	while (cmd[index])
	{
		if (cmd[index] == '=')
		{
			equal_sign++;
			if (is_ws(cmd[index - 1]))
				return (ft_dprintf(2,"minishell: export: `%s': not a valid identifier\n", &cmd[index]), 0);
			tmp_index = index - 1;
			while (tmp_index > 0 && !is_ws(cmd[tmp_index]))
			{
				if (ft_isalnum(cmd[tmp_index]) || is_char_in_set(cmd[tmp_index], "_"))
					tmp_index--;
				else
					return (ft_dprintf(2,"minishell: export: `%s': not a valid identifier\n"), 0);
			}
		}
		index++;
	}
	if (!equal_sign)
		return (0);
	return (1);
}

static int	var_exist(t_mshell *shell, char *var)
{
	t_envp	*temp;

	temp = shell->envp;
	while (temp)
	{
		if (ft_strcmp(var, temp->var_name) == 0)
			return (1);
		temp = temp->next;
	}
	return (0);
}

static void	add_to_env(t_mshell *shell, char *var, char *content)
{
	if (shell->envp && var_exist(shell, var))
		delete_envvar(&shell->envp, var);
	create_envp_list(shell, var, content);
	if (content)
		free(content);
	free(var);
}

static int	split_var(char *var, char **var_name, char **content)
{
	int	index;

	index = 0;
	index = (int)find_char_index(var, '=');
	if (index > 0)
	{
		*var_name = ft_substr(var, 0, index);
		if (!*var_name)
			return (0);
		*content = ft_substr(&var[index + 1], 0, ft_strlen(&var[index]));
		if (!*content)
			return (free(var_name), 0);
	}
	else
	{
		*var_name = ft_strdup(var);
		if (!*var_name)
			return (0);
	}
	return (1);
}

static void	get_var(t_mshell *shell, char *cmd)
{
	int		start;
	int		end;
	char	*var;
	char	*var_name;
	char	*content;

	start = 0;
	end = 1;
	var_name = NULL;
	content = NULL;
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
	if (!split_var(var, &var_name, &content))
		(free_struct(shell), exit(1));
	free(var);
	add_to_env(shell, var_name, content);
}

void	export_case(t_mshell *shell, char *cmd)
{
	char	**var_arr;
	char	*var;
	int		index;

	var_arr = ft_split(cmd, ' ');
	var = NULL;
	index = 1;

	while (var_arr[index])
	{
		var = ft_strjoin("export ", var_arr[index]);
		if (count_arr_size(var_arr) != 1 && is_var(var))
			get_var(shell, var);
		index++;
		free(var);
		var = NULL;
	}
	sort_env(shell);
	if (count_arr_size(var_arr) == 1 && ft_strncmp("export", cmd, ft_strlen(cmd)) == 0)
		print_export(shell->export);
	if (shell->export)
		free_export(&shell->export);
}
