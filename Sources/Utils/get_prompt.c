/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 23:46:36 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/06 12:24:45 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	temp = ft_strjoin("\n\033[3m", directory);
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
		"\033[1mminishell@42\033[0m:~\033[0;32m$\033[0m ");
	free(directory);
	if (!shell->prompt)
		return (free_struct(shell), exit(2));
}