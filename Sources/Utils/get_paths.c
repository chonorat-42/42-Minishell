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
	int		j;
	size_t	len;

	if (!envp)
		return (-1);
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

char	*get_envvar_content(char *envvar, unsigned int start)
{
	char	*result;
	size_t	len;

	len = ft_strlen((const char *)envvar);
	if (!envvar || (int)(len - start) < 0 || len == 0)
		return (NULL);
	result = ft_substr((const char *)envvar, start, len - start);
	if (!result)
		return (NULL);
	return (result);
}

void get_current_location(t_mshell *shell)
{
	if (getcwd(shell->current_loc, SIZE_MAX) == NULL)
	shell->current_loc[0] = '\0';
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
	char	*zero;

	temp = ft_split((const char *)str, ':');
	if (!temp)
		return (1);
	size = count_arr_size(temp);
	args->paths = malloc(sizeof(char *) * (size + 1));
	if (!args->paths)
		return (free_arr(temp), temp = NULL, 2);
	args->paths[size] = NULL;
	zero = get_envvar_content(temp[0], 5);
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

void	get_paths(t_mshell *shell, char **envp)
{
	int	path_index;

	shell->cmd_count = 0;
	path_index = find_envvar_index(envp, "PATH");
	shell->tok_lst = NULL;
	shell->cmd = NULL;
	if (path_index == -1)
	{
		shell->paths = NULL;
		return ;
	}
	if (fix_paths(envp[path_index], shell))
		return (free_struct(shell));
	get_current_location(shell);
}