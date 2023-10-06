/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:53:05 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/09/16 14:53:05 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*mettre les lines bruts
ajouter des \n
join
expand*/

// char	*join_dlist(t_dlist	*lst)
// {
// 	char	*res;
// 	char	*join;
// 	t_dlist	*temp;

// 	res = NULL;
// 	temp = lst;
// 	while (temp)
// 	{
// 		join = ft_strjoin(temp->content, "\n");
// 		if (!res)
// 			res = ft_strdup(join);
// 		else
// 			res = strjoin_free_first(res, join);
// 		free(join);
// 		temp = temp->next;
// 	}
// 	return (res);
// }

// void	heredoc(char *delimiter, int fd_in, t_envp *envp)
// {
// 	char	*line;
// 	char	*result;
// 	char	*join;
// 	t_dlist	*new;
	
// 	result = NULL;
// 	line = NULL;
// 	new = NULL;
// 	while (1)
// 	{
// 		line = readline(">");
// 		if (!ft_strcmp(line, delimiter))
// 			break ;
// 		else
// 			split_into_dlst(&new, line, ft_strlen(line), 0);
// 		free(line);
// 	}
// 	free(line);
// 	join = join_dlist(new);
// 	free_dlist(&new);
// 	if (find_char_index(join, '$') >= 0)
// 	{
// 		if (check_after_dollar(join))
// 		{
//     		result = expand_envvar(join, envp);
// 			ft_free_null(&join);
// 		}
// 	}
// 	else
// 		result = ft_strdup(join);
// 	ft_printf("in main. result = %s, fd_in = %d\n", result, fd_in);
// 	ft_dprintf(fd_in, "%s", result);
// 	free(result);
// 	free(join);
// }

void	heredoc(char *delimiter, int fd_in, t_envp *envp)
{
	char	*line;
	char	*result;
	char	*temp;
	
	result = NULL;
	line = NULL;
	while (1)
	{
		line = readline(">");
		if (!ft_strcmp(line, delimiter))
		{
			ft_dprintf(fd_in, "%s", result);
			ft_free_null(&result);
			ft_free_null(&line);
			close(fd_in);
			return ;
		}
		else
		{
			if (find_char_index(line, '$') >= 0)
			{
				if (check_after_dollar(line))
				{
    				temp = expand_envvar(line, envp);
					ft_free_null(&line);
				}
			}
			else
				temp = ft_strdup(line);
			line = strjoin_free_first(temp, "\n");
			if (result)
			{
				result = strjoin_free_both(result, line);
				// result = NULL;
				line = NULL;
			}
			else
			{
				result = ft_strdup(line);
				ft_free_null(&line);
			}
		}
		if (line)
			ft_free_null(&line);
	}
}

// void	heredocB(char *delimiter, int fd_in, t_envp *envp)
// {
// 	char	*line;
// 	char	*result;
// 	char	*temp;

// 	result = NULL;
// 	line = NULL;
// 	while (1)	
// 	{
// 		line = readline(">");
// 		if (!ft_strcmp(line, delimiter))
// 		{
// 			ft_dprintf(fd_in, "%s", result);
// 			ft_free_null(result);
// 			if (line)
// 				ft_free_null(line);
// 			close(fd_in);
// 			return ;
// 		}
// 		else
// 		{
// 			if (find_char_index(line, '$') >= 0)
// 			{
// 				if (check_after_dollar(line))
// 				{
//     				temp = expand_envvar(line, envp);
// 					ft_free_null(&line);
// 				}
// 			}
// 			else
// 				temp = ft_strdup(line);
// 			line = strjoin_free_first(temp, "\n");
// 			if (result)
// 			{
// 				result = strjoin_free_both(result, line);
// 				result = NULL;
// 				line = NULL;
// 			}
// 			else
// 			{
// 				result = ft_strdup(line);
// 				ft_free_null(&line);
// 			}
// 		}
// 		if (line)
// 			ft_free_null(&line);
// 	}
// }
