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

#include "../../../Includes/minishell.h"

void env_case(t_mshell *shell, char *cmd)
{
	size_t j;

	j = 0;
	if (ft_strlen(cmd) != 3)
		j = -1;
	while (++j < shell->envp_size)
	{
		if (shell->menvp[j])
			ft_printf("%s\n", shell->menvp[j]);
	}
}
