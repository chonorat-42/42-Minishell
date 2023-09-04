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

#include "minishell.h"

void	get_input_loop(t_mshell *shell)
{
	shell->input = NULL;
	while (1)
	{
		shell->input = readline("\033[1mminishell@42\033[0m:~\033[0;32m$\033[0m ");
		if (!shell->input)
			return (free_struct(shell), exit(2));
		else
			add_history(shell->input);
		parsing(shell);
		tokenizer(shell);
		execution(shell);
	}
}
