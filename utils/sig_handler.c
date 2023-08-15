/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 15:47:13 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/15 16:30:46 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"

// extern int g_signumm;

// void struct_init(t_mshell *shell)
// {
// 	shell->prompt = NULL;
// 	shell->cmd = NULL;
// 	shell->paths = NULL;
// 	shell->temp = NULL;
// 	shell->zero = NULL;
// 	shell->user = NULL;
// 	shell->session = NULL;
// 	shell->prompt = NULL;
// 	shell->join_user = NULL;
// 	shell->current_loc = NULL;
// }

// static void	ignore_sig(int signum, siginfo_t *info, void *context)
// {
// 	(void)context;
// 	(void)info;
// 	(void)signum;
// 	g_signumm = signum;
// 	ft_printf("\n");
// }

// void	sig_handler(void)
// {
// 	struct sigaction	signal;

// 	sigemptyset(&signal.sa_mask);
// 	signal.sa_sigaction = ignore_sig;
// 	signal.sa_flags = SA_RESTART;
// 	sigaction(SIGINT, &signal, NULL);
	// if (shell->prompt)
	// 	free(shell->prompt);
	// if (shell->join_user)
	// 	ft_printf("/n%s", shell->join_user);
// }