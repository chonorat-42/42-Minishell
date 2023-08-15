/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:19:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/14 15:50:37 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_index(char **envp, const char *str)
{
	int	j;
	size_t len;

	j = 0;
	len = ft_strlen(str);
	while (envp[j])
	{
		if (!ft_strncmp((const char *)envp[j], str, len))
			return (j);
		j++;
	}
	return (-1);
}

static char	*get_zero(char *str, unsigned int start)
{
	char	*result;
	size_t	len;

	len = ft_strlen((const char *)str);
	if (!str || len - start < 0 || len == 0)
		return (NULL);
	result = ft_substr((const char *)str, start, len - start);
	if (!result)
		return (NULL);
	return (result);
}

int get_current_location(t_mshell *shell, char **envp)
{
	int index;

	index = find_index(envp, "PWD=");
	shell->current_loc = get_zero(envp[index], ft_strlen("PWD="));
	if (!shell->current_loc)
		return (2);
	return (0);
}

int join_user_session(t_mshell *shell)
{
	char *temp;
	char *temp2;

	temp = ft_strjoin((const char *)shell->user, "@");
	if (!temp)
		return (1);
	temp2 = ft_strjoin((const char *)temp, shell->session);
	if (!temp2)
		return (free(temp), 2);
	shell->join_user = ft_strjoin((const char *)temp2, ":~$ ");
	if (!shell->join_user)
		return (free(temp), free(temp2), 3);
	return (free(temp), free(temp2), 0);

}

int get_session(t_mshell *shell, char **envp)
{
	int index;

	index = find_index(envp, "DESKTOP_SESSION=");
	shell->session = get_zero(envp[index], ft_strlen("DESKTOP_SESSION="));
	if (!shell->session)
		return (1);
	return (0);
}

int get_user_info(t_mshell *shell, char **envp)
{
	int index;

	index = find_index(envp, "USER=");
	shell->user = get_zero(envp[index], ft_strlen("USER="));
	if (!shell->user)
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

	args->temp = ft_split((const char *)str, ':');
	if (!args->temp)
		return (1);
	size = count_arr_size(args->temp);
	args->paths = malloc(sizeof(char *) * (size + 1));
	if (!args->paths)
		return (free_arr(args->temp), args->temp = NULL, 2);
	args->paths[size] = NULL;
	args->zero = get_zero(args->temp[0], 5);
	if (!args->zero)
		return (free_arr(args->temp), args->temp = NULL, free_arr(args->paths), args->paths = NULL, 3);
	args->paths[0] = add_ending_slash(args->zero);
	if (!args->paths[0])
		return (free(args->zero), free_arr(args->temp), args->temp = NULL, free_arr(args->paths), args->paths = NULL, 4);
	free(args->zero);
	j = 0;
	while (++j < size)
	{
		args->paths[j] = add_ending_slash(args->temp[j]);
		if (!args->paths[j])
			return (free_arr(args->temp), args->temp = NULL, free_arr(args->paths), args->paths = NULL, 3);
	}
	return (free_arr(args->temp), args->temp = NULL, 0);
}

int get_paths(t_mshell *shell, char **envp)
{
	int path_index;

	shell->cmd_count = 0;
	path_index = find_index(envp, "PATH=");
	shell->hist = NULL;
	shell->tok_lst = NULL;
	if (path_index == -1)
		return (1);
	if (fix_paths(envp[path_index], shell))
		return (2);
	if (get_user_info(shell, envp))
		return (3);
	if (get_session(shell, envp))
		return(4);
	if (join_user_session(shell))
		return (5);
	if (get_current_location(shell, envp))
		return (6);
	return (0);
}