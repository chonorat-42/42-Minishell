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

void	exec_sig(t_mshell *shell)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR
		|| signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		return (perror("signal"), free_struct(shell), exit(1));
}

static void	dfl_sig(int signum)
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

void	default_sig(t_mshell *shell)
{
	struct sigaction	sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_handler = dfl_sig;
	if (sigaction(SIGINT, &sig, NULL) == -1
		|| sigaction(SIGQUIT, &sig, NULL) == -1)
		return (perror("sigaction"), free_struct(shell), exit(1));
}
