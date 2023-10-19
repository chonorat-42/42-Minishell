/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:19:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 19:03:32 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_envvar_content(t_mshell *shell, t_envp *envp, char *to_find)
{
	t_envp	*temp;
	char	*result;

	temp = envp;
	result = NULL;
	while (temp)
	{
		if (!ft_strcmp(to_find, temp->var.name))
		{
			if (temp->var.content)
			{
				result = ft_strdup(temp->var.content);
				if (!result)
					return (free_struct(shell), exit(1), NULL);
			}
			else
				return (NULL);
			return (result);
		}
		temp = temp->next;
	}
	return (NULL);
}

static char	*add_ending_slash(char *str)
{
	char	*result;
	size_t	i;

	result = malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = '/';
	result[i + 1] = '\0';
	return (result);
}

static int	fix_paths(char *str, t_mshell *args)
{
	size_t	size;
	size_t	j;
	char	**temp;

	temp = ft_split((const char *)str, ':');
	if (!temp)
		return (1);
	size = ft_arr_size(temp);
	args->paths = malloc(sizeof(char *) * (size + 1));
	if (!args->paths)
		return (free_arr(temp), temp = NULL, 2);
	args->paths[size] = NULL;
	j = -1;
	while (++j < size)
	{
		args->paths[j] = add_ending_slash(temp[j]);
		if (!args->paths[j])
			return (free_arr(temp), temp = NULL, free_arr(args->paths),
				args->paths = NULL, 3);
	}
	return (free_arr(temp), temp = NULL, 0);
}

char	*get_envp_content(t_mshell *shell, t_envp *envp, char *to_find)
{
	t_envp	*temp;
	char	*res;

	temp = envp;
	while (temp)
	{
		if (!ft_strcmp(to_find, temp->var.name))
		{
			res = ft_strdup(temp->var.content);
			if (!res)
				return (free_struct(shell), exit(1), NULL);
			return (res);
		}
		temp = temp->next;
	}
	return (NULL);
}

void	get_paths(t_mshell *shell)
{
	char	*paths;

	if (shell->paths)
		free_arr(shell->paths);
	paths = get_envp_content(shell, shell->envp, "PATH");
	if (!paths)
		return ;
	if (fix_paths(paths, shell))
		return (free_struct(shell));
	free(paths);
}
