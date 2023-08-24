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

#include "../../../Includes/minishell.h"

void	unset_case(t_mshell *shell, char *str)
{
	int		index;
	char	*var;

	var = ft_substr(str, 6, ft_strlen(str));
	index = find_envvar_index(shell->menvp, var);
	if (index < 0)
		return ;
	if (!ft_strcmp(var, "PATH"))
	{
		free_arr(shell->paths);
		shell->paths = NULL;
	}
	free(shell->menvp[index]);
	shell->menvp[index] = NULL;
}