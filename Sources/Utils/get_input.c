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
	while (1)
	{
		sig_handler();
		get_prompt(shell);
		shell->input = readline(shell->prompt);
		if (!shell->input)
			return (ft_printf("exit\n"), free_struct(shell), exit(0));
		if (shell->input[0])
			add_history(shell->input);
		get_paths(shell);
		get_svar(shell);
		update_envp(shell);
		parsing(shell);
		tokenizer(shell);
		execution(shell);
		if (shell->prompt)
			free(shell->prompt);
	}
}
