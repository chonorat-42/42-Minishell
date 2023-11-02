/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_sig.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:22:02 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/11/02 13:33:54 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipe_signal(int signum)
{
	int	*freed;

	(void)signum;
	close_std_fds();
	free(adress_keeper(NULL));
	freed = fd_keeper(NULL);
	*freed = 1;
}

void	pipe_sig(t_mshell *shell)
{
	struct sigaction	sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_handler = pipe_signal;
	if (sigaction(SIGPIPE, &sig, NULL) == -1)
		return (perror("sigaction"), free_struct(shell), exit(1));
}
