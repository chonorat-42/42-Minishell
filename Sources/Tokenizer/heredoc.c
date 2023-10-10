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

void	add_newline_dlist(t_dlist **lst)
{
	t_dlist	*temp;
	char	*res;

	temp = *lst;
	while (temp)
	{
		res = ft_strjoin(temp->content, "\n");
		free(temp->content);
		temp->content = res;
		temp = temp->next;
	}
}

void	delimiter_found(t_mshell *shell, t_envp *envp, t_dlist *lst, int fd_in, int del_quote)
{
	char	*result;

	if (!del_quote)
		expand_dlist(shell, envp, &lst);
	add_newline_dlist(&lst);
	result = join_dlist(lst);
	if (result)
		ft_dprintf(fd_in, "%s", result);
	free(result);
	free_dlist(&lst);
}

void	heredoc(t_mshell *shell, char *delimiter, int fd_in, t_envp *envp)
{
	t_dlist	*lst;
	char	*line;
	int		del_quote;
	char	*new_del;
	char	*trim;

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
	while (1)
	{
		ft_dprintf(STDOUT_FILENO, ">");
		line = get_next_line(0);
		trim = ft_strtrim(line, "\n");
		free(line);
		line = trim;
		if (!ft_strcmp(line, delimiter))
		{
			delimiter_found(shell, envp, lst, fd_in, del_quote);
			return (free(line), free(delimiter));
		}
		else
		{
			split_into_dlst(&lst, line, ft_strlen(line), 0);
			free(line);
		}
	}
}
