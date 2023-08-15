/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 11:29:34 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/04/26 16:27:28 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen_gnl(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	check_nl(char *stash)
{
	size_t	i;

	if (!stash)
		return (0);
	i = 0;
	while (stash[i])
	{
		if (stash[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*malloc_free(int size, char *to_free)
{
	char	*str;

	str = malloc(sizeof(char) * size);
	if (!str)
	{
		if (to_free)
		{
			free(to_free);
			to_free = NULL;
		}
		return (NULL);
	}
	return (str);
}

char	*ft_strchr_gnl( char *s, int c)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (&s[i]);
		else
			i++;
	}
	if (s[i] == c)
		return (&s[i]);
	if ((char) c == '\0')
		return (&s[i]);
	return (0);
}

char	*ft_strjoin_gnl(char *buffer, char *stash)
{
	int		i;
	int		j;
	char	*result;

	i = -1;
	j = 0;
	if (!stash)
	{
		stash = malloc(sizeof(char) * 1);
		if (!stash)
			return (NULL);
		stash[0] = '\0';
	}
	if (!stash || !buffer)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(stash) + ft_strlen(buffer) + 1));
	if (!result)
		return (free(stash), stash = NULL, NULL);
	while (stash[++i])
		result[i] = stash[i];
	while (buffer[j])
		result[i++] = buffer[j++];
	result[ft_strlen(stash) + ft_strlen(buffer)] = '\0';
	free(stash);
	return (result);
}
