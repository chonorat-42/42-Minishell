/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_sig.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:48:46 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/22 13:47:51 by chonorat         ###   ########.fr       */
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

void	heredoc_sig(t_mshell *shell, int fd)
{
	struct sigaction	sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_handler = hd_sig;
	if (sigaction(SIGINT, &sig, NULL) == -1
		|| sigaction(SIGQUIT, &sig, NULL) == -1)
		return (perror("sigaction"), free_struct(shell), close(fd), exit(1));
}
