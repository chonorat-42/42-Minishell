/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 17:35:09 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 19:03:11 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_case(t_mshell *shell, char *str)
{
	int		index;
	char	*var;
	char	**to_unset;
	size_t	i;

	var = ft_substr(str, 6, ft_strlen(str));
	if (!var)
		return (free_struct(shell), exit (11));
	to_unset = ft_split(var, ' ');
	if (!to_unset)
		return (free(var), free_struct(shell), exit(12));
	i = 0;
	while (to_unset[i])
	{
		index = find_envvar_index(shell->menvp, to_unset[i]);
		if (index >= 0)
		{
			if (!ft_strcmp(var, "PATH"))
			{
				free_arr(shell->paths);
				shell->paths = NULL;
			}
			free(shell->menvp[index]);
			shell->menvp[index] = NULL;
		}
		i++;
	}
}