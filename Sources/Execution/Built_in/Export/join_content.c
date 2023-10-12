/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_content.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:00:00 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/11 16:25:45 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_join(t_var *var, char **arg, char **content)
{
	char	*temp;

	free(*arg);
	free(var->content);
	temp = ft_strjoin(var->name, "=");
	if (!temp)
		return (free(var->name), 0);
	free(var->name);
	*arg = ft_strjoin(temp, *content);
	if (!(*arg))
		return (free(*content), free(temp), 0);
	free(*content);
	free(temp);
	return (1);
}

static void	prepare_join(t_mshell *shell, char **arg, int index)
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
		return (free(var.name), free(new_content),
			free_struct(shell), exit(EXIT_FAILURE));
	join_content = ft_strjoin(var.content, new_content);
	if (!join_content)
		return (free(var.name), free(var.content), free(new_content),
			free_struct(shell), exit(EXIT_FAILURE));
	free(new_content);
	if (!get_join(&var, arg, &join_content))
		return (free_struct(shell), exit(EXIT_FAILURE));
}

void	check_plus(t_mshell *shell, char **arg)
{
	int	index;

	index = 1;
	while ((*arg)[index])
	{
		if ((*arg)[index] == '=' && (*arg)[index - 1])
			if ((*arg)[index - 1] == '+')
				return (prepare_join(shell, arg, index - 1));
		index++;
	}
}