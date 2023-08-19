/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:19:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/19 21:32:27 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

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
	int	j;
	size_t len;

	j = 0;
	len = ft_strlen(str);
	while (envp[j])
	{
		if (!ft_strncmp((const char *)envp[j], str, len))
		{
			if (compare_strings((char *)str, envp[j]))
				return (j);
		}
		j++;
	}
	return (-1);
}

static char	*get_zero(char *str, unsigned int start)
{
	char	*result;
	size_t	len;

	len = ft_strlen((const char *)str);
	if (!str || start > len || len == 0)
		return (NULL);
	result = ft_substr((const char *)str, start, len - start);
	if (!result)
		return (NULL);
	return (result);
}

int get_current_location(t_mshell *shell, char **envp)
{
	int index;

	index = find_envvar_index(envp, "PWD");
	if (index < 0)
		return (shell->current_loc = NULL, ft_printf("Could not find current location\n"), 1);
	shell->current_loc = get_zero(envp[index], ft_strlen("PWD="));
	if (!shell->current_loc)
		return (2);
	return (0);
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
	char **temp;
	char *zero;

	temp = ft_split((const char *)str, ':');
	if (!temp)
		return (1);
	size = count_arr_size(temp);
	args->paths = malloc(sizeof(char *) * (size + 1));
	if (!args->paths)
		return (free_arr(temp), temp = NULL, 2);
	args->paths[size] = NULL;
	zero = get_zero(temp[0], 5);
	if (!zero)
		return (free_arr(temp), temp = NULL, free_arr(args->paths), args->paths = NULL, 3);
	args->paths[0] = add_ending_slash(zero);
	if (!args->paths[0])
		return (free(zero), free_arr(temp), temp = NULL, free_arr(args->paths), args->paths = NULL, 4);
	free(zero);
	j = 0;
	while (++j < size)
	{
		args->paths[j] = add_ending_slash(temp[j]);
		if (!args->paths[j])
			return (free_arr(temp), temp = NULL, free_arr(args->paths), args->paths = NULL, 3);
	}
	return (free_arr(temp), temp = NULL, 0);
}

int get_paths(t_mshell *shell, char **envp)
{
	int path_index;

	shell->cmd_count = 0;
	path_index = find_envvar_index(envp, "PATH");
	shell->tok_lst = NULL;
	shell->cmd = NULL;
	if (path_index == -1)
		return (0);
	if (fix_paths(envp[path_index], shell))
		return (2);
	if (get_current_location(shell, envp))
		return (6);
	return (0);
}