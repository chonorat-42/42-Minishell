/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:19:34 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/23 13:38:18 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

static void	ignore_sig(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)context;
	//ft_printf("pid: %d\n", info->si_pid);
	//ft_printf("fd: %d\n", info->si_fd);
	if (!info->si_fd)
		ft_printf("\b\b  \n");
}

void	exec_sig(void)
{
	struct sigaction	signal;

	sigemptyset(&signal.sa_mask);
	signal.sa_flags = SA_RESTART | SA_SIGINFO;
	signal.sa_sigaction = ignore_sig;
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
}

static void	get_sig(int signum)
{
	if (signum == SIGINT)
	{
		ft_printf("\b\b  ");
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signum == SIGQUIT)
			ft_printf("\b\b  \b\b");
	g_status = 128 + signum;
}

void	sig_handler(void)
{
	struct sigaction	signal;

	sigemptyset(&signal.sa_mask);
	signal.sa_flags = SA_RESTART;
	signal.sa_handler = get_sig;
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
}
