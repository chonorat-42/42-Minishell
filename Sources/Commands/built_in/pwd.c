/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 22:44:37 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/19 22:52:04 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void	pwd_case(t_mshell *shell)
{
	get_current_location(shell);
	if (shell->current_loc[0])
		ft_printf("%s\n", shell->current_loc);
	else
		ft_printf("\n");
}