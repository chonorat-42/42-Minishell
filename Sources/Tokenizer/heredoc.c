/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:05:28 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/06 11:05:28 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long g_status;

void	heredoc_into_infile(t_dlist **lst)
{
	t_dlist	*temp;

	temp = *lst;
	while (ft_strcmp(temp->content, "<<"))
		temp = temp->next;
	free(temp->content);
	temp->content = ft_strdup("<");
	temp = temp->next;
	temp->content = ft_strdup("/tmp/temp.heredoc2");
}

void	delimiter_found(t_mshell *shell, t_dlist *lst, int fd_in, int del_quote)
{
	char	*result;

	if (!del_quote)
		expand_dlist(shell, shell->envp, &lst);
	result = join_dlist(lst);
	if (result)
		ft_dprintf(fd_in, "%s", result);
	free(result);
	free_dlist(&lst);
}

void	heredoc(t_mshell *shell, char *delimiter, int fd_in)
{
	t_dlist	*lst;
	char	*line;
	int		del_quote;
	char	*new_del;
	char	*trim;
	pid_t	child;
	// pid_t	wpid;

	del_quote = 0;
	if (is_char_in_set(delimiter[0], "\'\""))
	{
		del_quote = 1;
		new_del = remove_quotes(delimiter);
		free(delimiter);
		delimiter = new_del;
	}
	lst = NULL;
	line = NULL;
	child = fork();
	heredoc_sig();
	if (!child)
	{
		while (1)
		{
			ft_dprintf(STDOUT_FILENO, "> ");
			line = get_next_line(0);
			if (!line)
				return ((void)ft_putchar_fd('\n', 0));
			if (g_status == 130 || g_status == 131)
				return (free(line));
			trim = ft_strtrim(line, "\n");
			if (!ft_strcmp(trim, delimiter))
			{
				delimiter_found(shell, lst, fd_in, del_quote);
				return (free(line), free(trim), free(delimiter));
			}
			else
			{
				split_into_dlst(&lst, line, ft_strlen(line), 0);
				free(line);
				free(trim);
			}
		}
	}
	else
	{
		waitpid(child, (int *)&g_status, 0);
		// if (wpid == -1)
		// {
		// 	perror("waitpid");
		// 	exit(EXIT_FAILURE);
		// }
		if (WIFEXITED(g_status))
			g_status = WEXITSTATUS(g_status);
		else if (WIFSIGNALED(g_status))
		{
			g_status = WTERMSIG(g_status);
			if (g_status != 131)
			g_status += 128;
		}
	}
}
