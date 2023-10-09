/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 23:46:36 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/09 13:46:58 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//"\033[1mminishell@42\033[0m:~\033[0;32m$\033[0m "

int	history(char *input)
{
	int	index;

	if (!input[0])
		return (0);
	index = 0;
	while (input[index])
	{
		if (ft_isws(input[index]))
			index++;
		else
			return (1);
	}
	return (0);
}
/*
static char	*get_directory(t_mshell *shell)
{
	char	*directory;
	char	*temp;

	directory = get_envvar_content(shell->envp, "PWD");
	if (!directory)
	{
		directory = ft_strdup("?");
		if (!directory)
			return (free_struct(shell), NULL);
	}
	temp = ft_strjoin("\n\033[0;37m", directory);
	if (!temp)
		return (free(directory), NULL);
	free(directory);
	directory = ft_strjoin(temp, "\033[0m\n");
	if (!directory)
		return (free(temp), NULL);
	free(temp);
	return (directory);
}

void	get_prompt(t_mshell *shell)
{
	char	*directory;

	directory = get_directory(shell);
	if (!directory)
		return (free_struct(shell), exit(2));
	shell->prompt = ft_strjoin(directory, 
		"> \033[0;32m$\033[0m ");
	free(directory);
	if (!shell->prompt)
		return (free_struct(shell), exit(2));
}*/

static void	get_color(t_mshell *shell)
{
	if (shell->exit_status == 0)
	{
		shell->prompt = ft_strdup("> \033[0;32m$\033[0m ");
		if (!shell->prompt)
			return (free_struct(shell), exit(2));
	}
	else
	{
		shell->prompt = ft_strdup("> \033[0;31m$\033[0m ");
		if (!shell->prompt)
			return (free_struct(shell), exit(2));
	}
}

static void	print_upper(t_mshell *shell)
{
	struct stat	stat;
	char		*path;
	char		*home;
	char		*temp;

	path = get_envvar_content(shell, shell->envp, "PWD");
	home = get_envvar_content(shell, shell->envp, "HOME");
	if (home && ft_strlen(home) > 1 && home[ft_strlen(home) - 1] == '/')
		home[ft_strlen(home) - 1] = '\0';
	if (path && home && ft_strncmp(home, path, ft_strlen(home)) == 0
		&& lstat(home, &stat) == 0)
	{
		temp = ft_substr(path, ft_strlen(home), 
			ft_strlen(&path[ft_strlen(home)]));
		if (!temp)
			return (free_struct(shell), exit(2));
		ft_printf("\033[0;37m~\033[0m");
		if (temp[0] && temp[0] != '/')
			ft_printf("\033[0;37m/\033[0m");
		ft_printf("\033[0;37m%s\033[0m", temp);
		free(temp);
	}
	else if (path)
		ft_printf("\033[0;37m%s\033[0m", path);
	return (free(path), free(home));
}

char	*get_prompt(t_mshell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
	ft_printf("\n\033[1mMinishell \033[0m");
	print_upper(shell);
	ft_printf("\n");
	get_color(shell);
	return (shell->prompt);
}