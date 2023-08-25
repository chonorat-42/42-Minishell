/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:25:00 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:14 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

static size_t	count_word(char const *s, char c)
{
	size_t	count;
	size_t	index;

	index = 0;
	count = 0;
	while (s[index] == c && s[index])
		index++;
	while (s[index])
	{
		if (s[index] == c)
		{
			count++;
			while (s[index] == c && s[index])
				index++;
		}
		else
			index++;
	}
	if (index > 0 && s[index - 1] != c)
		count++;
	return (count);
}

static size_t	count_char(char const *s, char c, int index)
{
	size_t	count;

	count = 0;
	while (s[index] && s[index] != c)
	{
		count++;
		index++;
	}
	return (count);
}

static char	**free_malloc(char **str)
{
	int	index;

	index = 0;
	while (str[index])
	{
		free(str[index]);
		index++;
	}
	free(str);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**new_str;
	size_t	index;
	size_t	index_j;

	if (!s)
		return (NULL);
	new_str = (char **)malloc((count_word(s, c) + 1) * sizeof(char *));
	if (!new_str)
		return (NULL);
	index = 0;
	index_j = 0;
	while (index_j < count_word(s, c))
	{
		while (s[index] == c)
			index++;
		new_str[index_j] = ft_substr(s, index, count_char(s, c, index));
		if (!new_str[index_j])
			return (free_malloc(new_str));
		index += count_char(s, c, index);
		index_j++;
	}
	new_str[index_j] = 0;
	return (new_str);
}
