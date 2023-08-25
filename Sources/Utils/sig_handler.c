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

#include "../../Includes/minishell.h"

extern int	g_signum;

static void	get_sig(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	g_signum = signum;
	if (signum == SIGINT)
		ft_printf("\n\033[1mminishell\033[0m:~\033[0;32m$\033[0m ");
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
