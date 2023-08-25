/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 14:29:08 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:44 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

static int	check_set(char c, char const *set)
{
	int	index;

	index = 0;
	while (set[index])
	{
		if (c == set[index])
			return (1);
		index++;
	}
	return (0);
}

static int	check_onlyset(char const *s1, char const *set)
{
	int	index_j;

	index_j = 0;
	while (s1[index_j])
	{
		if (check_set(s1[index_j], set) == 0)
			return (0);
		index_j++;
	}
	return (1);
}

static char	*str_cpy(char *dst, char const *src, int index, int index_j)
{
	int	index_k;

	index_k = 0;
	while (index <= index_j)
		dst[index_k++] = src[index++];
	dst[index_k] = '\0';
	return (dst);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new_str;
	int		index;
	int		index_j;

	index = 0;
	index_j = 0;
	new_str = NULL;
	if (!s1 || !set)
		return (NULL);
	if (check_onlyset(s1, set) == 1)
		return (ft_strdup(""));
	while (check_set(s1[index], set))
		index++;
	while (s1[index_j + 1])
		index_j++;
	while (check_set(s1[index_j], set))
		index_j--;
	new_str = (char *)malloc(sizeof(char) * ((index_j - index) + 2));
	if (!new_str)
		return (NULL);
	return (str_cpy(new_str, s1, index, index_j));
}
