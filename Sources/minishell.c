/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 10:35:22 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 18:28:10 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signum;

int main(int argc, char **argv, char **envp)
{
	t_mshell	shell;

	(void)argv;
	if (argc != 1)
		return (ft_printf("Error\nMinishell does not take arguments\n"), 1);
	sig_handler();
	get_envp(&shell, envp);
	get_paths(&shell, shell.menvp);
	get_input_loop(&shell);
}
