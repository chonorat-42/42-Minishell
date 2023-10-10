/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 12:54:35 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/10 12:54:35 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

static void	hd_sig(int signum)
{
	(void)signum;
	g_status = 130;
	ft_putstr_fd("\b\b  \b\b", 0);
	ft_putstr_fd("\b\b  \b\b", 2);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	heredoc_sig(void)
{
	struct sigaction	signal;

	sigemptyset(&signal.sa_mask);
	signal.sa_flags = SA_RESTART;
	signal.sa_handler = hd_sig;
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
}

static void	ignore_sig(int signum)
{
	(void)signum;
	ft_putstr_fd("\b\b  ", 0);
}

void	exec_sig(void)
{
	struct sigaction	signal;

	sigemptyset(&signal.sa_mask);
	signal.sa_flags = SA_RESTART;
	signal.sa_handler = ignore_sig;
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
}

static void	get_sig(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\b\b  ", 0);
		ft_putstr_fd("\n", 0);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signum == SIGQUIT)
			ft_putstr_fd("\b\b  \b\b", 0);
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
