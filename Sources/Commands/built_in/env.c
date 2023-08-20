/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:16:14 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/19 19:10:35 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void env_case(t_mshell *shell)
{
	size_t j;

	j = -1;
	while (++j < shell->envp_size)
	{
		if (shell->menvp[j])
			ft_printf("%s\n", shell->menvp[j]);
	}
}
