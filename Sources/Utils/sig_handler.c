/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:19:34 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/19 14:54:02 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_signum;

static void	get_sig(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	g_signum = signum;
	if (signum == SIGINT)
		ft_printf("\nminishell:~$ ");
}

void	sig_handler(void)
{
	struct sigaction	sig;

	g_signum = 0;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_sigaction = get_sig;
	sigaction(SIGINT, &sig, NULL);
	signal(SIGQUIT, SIG_IGN);
}
