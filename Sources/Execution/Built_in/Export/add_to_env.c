/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:26:40 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/11 15:40:14 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

int	get_join(t_var *var, char **arg, char *content)
{
	char	*temp;

	free(*arg);
	temp = ft_strjoin(var->name, "=");
	if (!temp)
		return (free(var->name), 0);
	free(var->name);
	*arg = ft_strjoin(temp, content);
	if (!(*arg))
		return (free(temp), 0);
	free(temp);
	return (1);
}

void	prepare_join(t_mshell *shell, char **arg, int index)
{
	t_var	var;
	char	*new_content;
	char	*join_content;

	var.name = ft_substr(*arg, 0, index);
	if (!var.name)
		return (free_struct(shell), exit(EXIT_FAILURE));
	var.content = NULL;
	new_content = ft_substr(*arg, index + 2, ft_strlen(&(*arg)[index + 2]));
	if (!new_content)
		return (free(var.name), free_struct(shell), exit(EXIT_FAILURE));
	if (var_exist(shell, var.name))
		var.content = get_envvar_content(shell, shell->envp, var.name);
	if (!var.content)
		var.content = ft_strdup("");
	if (!var.content)
		return (free(var.name), free(new_content));
	join_content = ft_strjoin(var.content, new_content);
	if (!join_content)
		return (free(var.name), free(var.content), free(new_content),
			free_struct(shell), exit(EXIT_FAILURE));
	free(var.content);
	if (!get_join(&var, arg, join_content))
		return (free_struct(shell), free(new_content), exit(EXIT_FAILURE));
	ft_printf("%s\n", *arg);
	return (free(new_content), free(join_content));
}

int	is_var(char *arg)
{
	int	index;

	if (!ft_isalpha(arg[0]) && !is_char_in_set(arg[0], "_"))
	{
		g_status = 1;
		return (builtin_error("export", arg, 0), 0);
	}
	index = 1;
	while (arg[index])
	{
		if (arg[index] == '=')
			return (1);
		if (arg[index] == '+')
			if (arg[index + 1] == '=' && arg[index + 1])
				return (1);
		if (!ft_isalnum(arg[index]) && !is_char_in_set(arg[index], "_"))
		{
			g_status = 1;
			return (builtin_error("export", arg, 0), 0);
		}
		index++;
	}
	return (1);
}

static void	add_to_env(t_mshell *shell, t_var *new)
{
	if ((var_exist(shell, new->name) && new->readable == 0)
		|| ft_strncmp(new->name, "_", ft_strlen(new->name)) == 0
		|| ft_strncmp(new->name, "?", ft_strlen(new->name)) == 0)
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

static int	split_var(char *cmd, t_var *new)
{
	int	index;

	index = ft_char_index(cmd, '=');
	if (index > 0)
	{
		new->name = ft_substr(cmd, 0, index);
		if (!new->name)
			return (0);
		new->content = ft_substr(&cmd[index + 1], 0,
				ft_strlen(&cmd[index + 1]));
		if (!new->content)
			return (free(new->name), 0);
	}
	else
	{
		new->name = ft_strdup(cmd);
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
	index = ft_char_index(var, '=');
	if (index > 0)
		new->readable = 1;
}

void	get_var(t_mshell *shell, char *cmd)
{
	t_var	new;

	get_readable(cmd, &new);
	if (!split_var(cmd, &new))
		(free_struct(shell), exit(EXIT_FAILURE));
	add_to_env(shell, &new);
}
