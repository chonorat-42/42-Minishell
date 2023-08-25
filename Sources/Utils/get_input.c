/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 17:24:22 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 18:17:10 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void    get_input_loop(t_mshell *shell)
{
    char    *line;

    while (1)
	{
		ft_printf("\033[1mminishell\033[0m:~\033[0;32m$\033[0m ");
		line = get_next_line(0);
		if (!line)
			return (free_struct(shell), exit(2));
		shell->prompt = ft_strtrim((const char *)line, "\n\t\v\f\r ");
		if (!shell->prompt)
			return (free_struct(shell), exit(3));
		free(line);
        parsing(shell);
        tokenizer(shell);
        execution(shell);
	}
    free(shell->prompt);
	shell->prompt = NULL;
}