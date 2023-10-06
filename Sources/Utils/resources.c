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

long int find_char_index(char *str, int c)
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

void	print_arr(char **arr)
{
	int j;

	if (DEBUG)
	{
		j = -1;
		while(arr[++j])
			printf("%s\n", arr[j]);
	}
}
