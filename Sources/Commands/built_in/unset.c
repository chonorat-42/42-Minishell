/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 17:35:09 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/19 19:15:05 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void unset_case(t_mshell *shell, char *str)
{
	int index;
	char *var;

	var = ft_substr(str, 6, ft_strlen(str));
	index = find_envvar_index(shell->menvp, var);
	if (index < 0)
		return ;
	free(shell->menvp[index]);
	shell->menvp[index] = NULL;
}