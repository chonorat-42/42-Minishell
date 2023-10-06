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


- export (diff export env ?) DONE ?
- gerer simple quotes echo
- factorisation + cleaning
- redirection STDIN
- readline a la place de GNL
- $?
- executable PATH relatif ou absolu ->ex : ./minishell
- expand meme entre quotes


remake parsing
1) decouper selon metacaracteres
2) Quotes
3) Expand
*/

long long	g_status;


static char	**get_exit(t_mshell *shell)
{
	char	**arr;

	arr = malloc(sizeof(char *) * 2);
	if (!arr)
		return (free_struct(shell), exit(2), NULL);
	arr[1] = NULL;
	arr[0] = ft_strdup("exit");
	if (!arr[0])
		return (free(arr), free_struct(shell), exit(2), NULL);
	return (arr);
}

void	init_shell(t_mshell *shell)
{
	shell->input = NULL;
	shell->cmd = NULL;
	shell->prompt = NULL;
	shell->menvp = NULL;
	shell->paths = NULL;
	shell->current_loc = NULL;
	shell->exit = NULL;
	shell->exit_status = 0;
	shell->envp_size = 0;
	shell->tok_lst = NULL;
	shell->envp = NULL;
	shell->export = NULL;
	g_status = 0;
}

int main(int argc, char **argv, char **envp)
{
	t_mshell	shell;

	if (argc != 1)
		return (ft_printf("Error\nMinishell does not take arguments\n"), 1);
	init_shell(&shell);
	get_envp(&shell, envp, argv);
	update_shlvl(&shell);
	shell.exit = get_exit(&shell);
	get_input_loop(&shell);
	return (shell.exit_status);
}
