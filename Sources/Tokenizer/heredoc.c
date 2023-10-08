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
	free(temp->content);
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

void	heredoc(t_mshell *shell, char *delimiter, int fd_in, t_envp *envp)
{
	t_dlist	*lst;
	char	*result;
	char	*line;
	
	(void)shell;
	(void)envp;
	lst = NULL;
	result = NULL;
	line = NULL;
	while (1)
	{
		line = readline(">");
		if (!ft_strcmp(line, delimiter))
		{
			expand_dlist(shell, &lst);
			add_newline_dlist(&lst);
			result = join_dlist(lst);
			ft_dprintf(fd_in, "%s", result);
			free(result);
			free(line);
			free_dlist(&lst);
			return ;
		}
		else
		{
			split_into_dlst(&lst, line, ft_strlen(line), 0);
			free(line);
		}
	}
}
