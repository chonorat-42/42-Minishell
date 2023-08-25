/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 18:13:13 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 18:24:53 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    parsing(t_mshell *shell)
{
    if (shell->input[0] != '\0')
		{
			if (expand(shell, shell->input) == 1)
                return (free(shell->input), get_input_loop(shell));
			manage_quotes(shell);
		}
    else
    {
        free(shell->input);
        shell->input = NULL;
        get_input_loop(shell);
    }	
}
