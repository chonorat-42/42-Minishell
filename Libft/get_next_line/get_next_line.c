/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:21:18 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/17 16:55:10 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/libft.h"

char	*send_to_line(char *stash)
{
	size_t	i;
	char	*s;

	i = 0;
	if (stash[i] == '\0')
		return (NULL);
	while (stash[i] != '\n' && stash[i])
		i++;
	s = malloc(sizeof(char) * (i + 1 + check_nl(stash)));
	if (!s)
		return (NULL);
	i = 0;
	while (stash[i] != '\n' && stash[i])
	{
		s[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
	{
		s[i] = stash[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

char	*stash_ready(char *stash)
{
	size_t	i;
	size_t	j;
	char	*temp;

	i = 0;
	j = 0;
	if (!stash)
		return (NULL);
	while (stash[i] != '\n' && stash[i])
		i++;
	if (stash[i] == '\0')
	{
		free(stash);
		return (NULL);
	}
	temp = malloc((sizeof(char) * (ft_strlen(stash) - i + 1)));
	if (!temp)
		return (free(stash), stash = NULL, NULL);
	i++;
	while (stash[i])
		temp[j++] = stash[i++];
	temp[j] = '\0';
	return (free(stash), stash = NULL, temp);
}

char	*read_save(int fd, char *stash)
{
	int		char_count;
	char	buffer[BUFFER_SIZE + 1];

	char_count = 1;
	buffer[0] = 0;
	while (char_count > 0 && !check_nl(stash))
	{
		char_count = read(fd, buffer, BUFFER_SIZE);
		if (stash && ft_strchr(stash, '\0') && !char_count
			&& !buffer[0] && !check_nl(stash))
			break ;
		if ((!char_count && !buffer[0]) || char_count == -1)
		{
			if (stash)
				free(stash);
			return (NULL);
		}
		buffer[char_count] = '\0';
		stash = ft_strjoin_gnl(buffer, stash);
		if (!stash)
			return (NULL);
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) == -1)
	{
		if (stash)
		{
			free(stash);
			stash = NULL;
		}
		return (NULL);
	}
	stash = read_save(fd, stash);
	if (!stash)
		return (NULL);
	line = send_to_line(stash);
	if (!line)
		return (free(stash), stash = NULL, NULL);
	stash = stash_ready(stash);
	return (free(stash), stash = NULL, line);
}