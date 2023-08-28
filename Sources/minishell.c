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

/*to do :
- debugger bin_exec double execution DONE
- builtin sans arg KO DONE
- refaire expand DONE
- gestion des pipes DONE 28/08


- export (diff export env ?) DONE ?
- gerer simple quotes echo
- factorisation + cleaning
- redirection STDIN
- readline a la place de GNL
- $?
- executable PATH relatif ou absolu ->ex : ./minishell
*/

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
