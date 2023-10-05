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


void	heredoc(char *delimiter, int fd_in, t_envp *envp)
{
	char	*line;
	char	*result;
	char	*temp;

	ft_printf("IN HEREDOC, DELIMITER = %s\n\n", delimiter);
	
	result = NULL;
	line = NULL;
	while (1)
	{
		line = readline(">");
		if (!ft_strcmp(line, delimiter))
		{
			ft_dprintf(fd_in, "%s", result);
			ft_free_null(result);
			if (line)
				ft_free_null(line);
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
					ft_free_null(line);
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
				ft_free_null(line);
			}
		}
		// if (line)
		// 	ft_free_null(line);
	}
}

void	heredocB(char *delimiter, int fd_in, t_envp *envp)
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
			ft_free_null(result);
			if (line)
				ft_free_null(line);
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
					ft_free_null(line);
				}
			}
			else
				temp = ft_strdup(line);
			line = strjoin_free_first(temp, "\n");
			if (result)
			{
				result = strjoin_free_both(result, line);
				result = NULL;
				line = NULL;
			}
			else
			{
				result = ft_strdup(line);
				ft_free_null(line);
			}
		}
		if (line)
			ft_free_null(line);
	}
}
