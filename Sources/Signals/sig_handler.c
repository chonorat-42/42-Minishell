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
	ft_putstr_fd("\b\b  \b\b", 0);
	if (signum == SIGINT)
	{
		g_status = 128 + signum;
		ft_putstr_fd("\n", 0);
		free_struct(adress_keeper(NULL));
		free_dlist(dlist_keeper(NULL));
		close(*fd_keeper(NULL));
		exit(g_status);
	}
}

void	heredoc_sig(void)
{
	struct sigaction	sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_handler = hd_sig;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGQUIT, &sig, NULL);
}

static void	ign_sig(int signum)
{
	(void)signum;
}

void	ignore_sig(void)
{
	struct sigaction	sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_handler = ign_sig;
	sigaction(SIGINT, &sig, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	exec_sig(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
		g_status = 128 + signum;
	}
	else if (signum == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 0);
}

void	sig_handler(void)
{
	struct sigaction	sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_handler = get_sig;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGQUIT, &sig, NULL);
}
