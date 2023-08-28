/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:16:14 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/20 18:51:16 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void env_case(t_mshell *shell, int fd)
{
	size_t j;

	j = 0;
	while (j < shell->envp_size)
	{
		if (shell->menvp[j])
			ft_dprintf(fd, "%s\n", shell->menvp[j]);
		j++;
	}
}
