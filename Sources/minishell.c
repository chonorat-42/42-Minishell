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

/*to do :
- debugger bin_exec double execution DONE
- builtin sans arg KO DONE
- refaire expand DONE
- gestion des pipes DONE 28/08
- export (diff export env ?) DONE
- readline a la place de GNL DONE

- $? IN_WORK

- gerer simple quotes echo
- factorisation + cleaning
- redirection STDIN
- executable PATH relatif ou absolu ->ex : ./minishell
- expand meme entre quotes


remake parsing
1) decouper selon metacaracteres
2) Quotes
3) Expand
*/

int	g_status = 0;

int main(int argc, char **argv, char **envp)
{
	t_mshell	shell;

	(void)argv;
	if (argc != 1)
		return (ft_printf("Error\nMinishell does not take arguments\n"), 1);
	sig_handler();
	get_envp(&shell, envp);
	get_paths(&shell);
	get_input_loop(&shell);
}
