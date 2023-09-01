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

size_t find_char_index(char *str, int c)
{
	size_t i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

int compare_strings(char *str, char *envs)
{
	size_t i;

	i = find_char_index(envs, '=');
	if (ft_strlen(str) == i)
		return (1);
	return (0);
}

int	find_envvar_index(char **envp, const char *str)
{
	int		j;
	char	*temp;

	if (!envp)
		return (-1);
	j = 0;
	while (envp[j])
	{
		temp = ft_substr(envp[j], 0, find_char_index(envp[j], '='));
		if (!ft_strcmp(temp, str))
				return (j);
		j++;
	}
	return (-1);
}

char	*get_envvar_content(t_envp *envp, char *to_find)
{
	t_envp	*temp;
	char	*result;

	temp = envp;
	while (temp->next)
	{
		if (!ft_strcmp(to_find, temp->var_name))
		{
			result = ft_strdup(temp->var_cont);
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
	// char	*zero;

	temp = ft_split((const char *)str, ':');
	if (!temp)
		return (1);
	size = count_arr_size(temp);
	args->paths = malloc(sizeof(char *) * (size + 1));
	if (!args->paths)
		return (free_arr(temp), temp = NULL, 2);
	args->paths[size] = NULL;
	// zero = get_envvar_content(temp[0], 5);
	// if (!zero)
	// 	return (free_arr(temp), temp = NULL, free_arr(args->paths), args->paths = NULL, 3);
	// args->paths[0] = add_ending_slash(zero);
	// if (!args->paths[0])
	// 	return (free(zero), free_arr(temp), temp = NULL, free_arr(args->paths), args->paths = NULL, 4);
	// free(zero);
	j = -1;
	while (++j < size)
	{
		args->paths[j] = add_ending_slash(temp[j]);
		if (!args->paths[j])
			return (free_arr(temp), temp = NULL, free_arr(args->paths), args->paths = NULL, 3);
	}
	return (free_arr(temp), temp = NULL, 0);
}

char	*get_envp_content(t_envp *envp, char *to_find)
{
	t_envp	*temp;
	char	*res;

	temp = envp;
	while (temp->next && ft_strcmp(to_find, temp->var_name))
		temp = temp->next;
	res = ft_strdup(temp->var_cont);
	return (res);
}

void	get_paths(t_mshell *shell)
{
	char *paths;

	paths = get_envp_content(shell->envp, "PATH");
	if (fix_paths(paths, shell))
		return (free_struct(shell));
	print_env(shell->envp);
	free(paths);
}