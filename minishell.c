/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 10:35:22 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/15 16:30:24 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

	// int g_signumm;


int madd_history(t_mshell *shell)
{
	t_list *new;

	new = malloc(sizeof(*new));
	if (!shell->hist)
	{
		shell->hist = new;
		shell->hist->cmd = ft_strdup((const char *)shell->prompt);
		if (!shell->hist->cmd)
			return (1);
		shell->hist->next = NULL;
	}
	else
	{	
		new->next = shell->hist;
		shell->hist = new;
		shell->hist->cmd = ft_strdup((const char *)shell->prompt);
		if (!shell->hist->cmd)
			return (1);
	}
	return (0);
}


int main(int argc, char **argv, char **envp)
{
	t_mshell shell;

	(void)argc;
	(void)argv;

	// pid_t childcmd;
	char *line;
	
	// g_signumm = 0;
	struct_init(&shell);

	// sig_handler();
	if (get_paths(&shell, envp))
		return (1);
	ft_printf("path[0] = %s\n", shell.paths[0]);
	while (1)
	{
		// if (g_signumm == SIGINT)
		{
			ft_dprintf(0, "signum = %d\n", g_signumm);
			// g_signumm = 0;
		}
		ft_printf("%s ", shell.join_user);
		line = get_next_line(0);
		if (!line)
			return (free_struct(&shell), 1);
		shell.prompt = ft_strtrim((const char *)line, "\n");
		free(line);
		if (!shell.prompt)
			return (free_struct(&shell), 1);
		tokenizer(&shell);
		madd_history(&shell);

		execution(&shell, envp);
		// shell.cmd = ft_split((const char *)shell.prompt, ' ');
		// if (!shell.cmd)
		// 	return (free_struct(&shell), 1);
		// if (!ft_strncmp((const char *)shell.prompt, "exit", 4))
		// 	return (free_struct(&shell), 2);
		// else if (!ft_strncmp((const char *)shell.prompt, "echo", 4))
		// 			echo_case(shell.prompt);
		// else if (!ft_strncmp((const char *)shell.prompt, "cd", 2))
		// 		cd_case(&shell);
		// else
		// {
		// 	shell.cmd_count++;
		// 	childcmd = fork();
		// 	if (childcmd == 0)
		// 	{
		// 		if(env_case(shell, envp))
		// 			return (free_struct(&shell), 1);
		// 	}
		// }
		free(shell.prompt);
		shell.prompt = NULL;
		// free_arr(shell.cmd);
		// waitpid(childcmd, NULL, 0);
	}
	//waitpid(childcmd, NULL, 0);
	free_struct(&shell);
}
