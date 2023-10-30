/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ignore_sig.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:48:03 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/22 13:48:25 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ign_sig(int signum)
{
	(void)signum;
}

void	ignore_sig(t_mshell *shell)
{
	struct sigaction	sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_handler = ign_sig;
	if (sigaction(SIGINT, &sig, NULL) == -1)
		return (perror("sigaction"), free_struct(shell), exit(1));
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (perror("signal"), free_struct(shell), exit(1));
}
