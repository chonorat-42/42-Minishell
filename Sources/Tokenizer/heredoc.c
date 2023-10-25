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

extern long long	g_status;

void	heredoc_into_infile(t_dlist **lst)
{
	t_dlist	*temp;

	temp = *lst;
	while (ft_strcmp(temp->content, "<<"))
		temp = temp->next;
	free(temp->content);
	temp->content = ft_strdup("<");
	temp = temp->next;
	if (temp->content)
		free(temp->content);
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

static char	*get_hd_input(t_mshell *shell, char *delim, int fd, t_dlist *lst)
{
	char	*line;

	line = get_next_line(0);
	if (!line)
		return ((void)ft_putchar_fd('\n', 0),
			show_error(delim, HDOC, 0),
			free_dlist(&lst), free_struct(shell), close(fd), exit(0), NULL);
	return (line);
}

static void	heredoc_loop(t_mshell *shell, char *delim, int fd_in, int del)
{
	char	*line;
	char	*trim;
	t_dlist	*lst;

	line = NULL;
	trim = NULL;
	lst = NULL;
	heredoc_sig(shell, fd_in);
	dlist_keeper(&lst);
	while (1)
	{
		ft_dprintf(STDOUT_FILENO, "> ");
		line = get_hd_input(shell, delim, fd_in, lst);
		trim = ft_strtrim(line, "\n");
		if (!trim)
			return (close(fd_in), free_struct(shell), free(line), exit(1));
		if (!ft_strcmp(trim, delim))
			return (delimiter_found(shell, lst, fd_in, del), close(fd_in),
				multifree(line, trim, 0, 0), free_struct(shell), exit(0));
		else
		{
			split_into_dlst(&lst, line, ft_strlen(line), 0);
			multifree(line, trim, 0, 0);
		}
	}
}

void	heredoc(t_mshell *shell, char *delim, int fd_in)
{
	int		del_quote;
	char	*new_del;
	pid_t	child;

	del_quote = 0;
	if (!g_status)
	{
		if (is_char_in_set(delim[0], "\'\""))
		{
			del_quote = 1;
			new_del = remove_quotes(delim);
			free(delim);
			delim = new_del;
		}
		ignore_sig(shell);
		fd_keeper(&fd_in);
		child = fork();
		if (!child)
			heredoc_loop(shell, delim, fd_in, del_quote);
		else
		{
			if (waitpid(child, (int *)&g_status, 0) == -1)
				return (perror("waitpid"), exit(EXIT_FAILURE));
			get_fork_status();
		}
	}
}
