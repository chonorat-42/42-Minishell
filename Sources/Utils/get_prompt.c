/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 23:46:36 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/11 17:42:27 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

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
	ft_printf("\n\033[1mMinishell \033[0m");
	print_upper(shell);
	ft_printf("\n");
	if (g_status != 0)
		return ("> \033[0;31m$\033[0m ");
	return ("> \033[0;32m$\033[0m ");
}
