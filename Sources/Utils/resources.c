/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resources.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:34:33 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/17 16:50:41 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_isws(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isws(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	get_fork_status(void)
{
	if (WIFEXITED(g_status))
		g_status = WEXITSTATUS(g_status);
	else if (WIFSIGNALED(g_status))
	{
		g_status = WTERMSIG(g_status);
		if (g_status != 131)
			g_status += 128;
	}
}

long int	find_char_index(char *str, int c)
{
	long int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
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
			return (free(temp), j);
		free(temp);
		j++;
	}
	return (-1);
}

void	move_to_next_quote(char *str, size_t *i, char c)
{
	(*i)++;
	while (str[(*i)] && str[(*i)] != c)
		(*i)++;
}
