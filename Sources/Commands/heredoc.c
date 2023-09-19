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

void	heredoc(char *delimiter, int fd_in)
{
	char	*line;
	char	*result;

	result = NULL;
	while (1)
	{
		line = readline(">");
		if (!strcmp(line, delimiter))
		{
			ft_dprintf(fd_in, "%s", result);
			return ;
		}
		else
		{
			line = strjoin_free_first(line, "\n");
			if (result)
				result = strjoin_free_both(result, line);
			else
			{
				result = ft_strdup(line);
				free(line);
			}
		}
	}
}
