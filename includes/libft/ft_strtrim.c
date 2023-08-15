/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 16:06:30 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/03/30 14:53:31 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_count_first(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (set[j])
	{
		if (s1[i] == set[j])
		{
			i++;
			j = 0;
		}
		else
			j++;
	}
	return (i);
}

static unsigned int	ft_count_last(char const *s1, char const *set,
unsigned int len)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;

	i = 0;
	j = len - 1;
	k = 0;
	while (set[k])
	{
		if (s1[j] == set[k])
		{
			i++;
			j--;
			k = 0;
		}
		else
			k++;
	}
	return (i);
}

static char	*ft_str_duplicate(char const *s1,
		char const *set, char *str, unsigned int length)
{
	unsigned int	i;
	unsigned int	j;

	i = ft_count_first(s1, set);
	j = 0;
	while (j < length)
	{
		str[j] = s1[i];
		j++;
		i++;
	}
	str[j] = '\0';
	return (str);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	unsigned int	usable_length;
	unsigned int	total_length;
	char			*str;

	if (!s1 || (!s1 && !set))
		return (ft_strdup(""));
	total_length = ft_strlen(s1);
	if (ft_count_first(s1, set) == total_length)
		return (ft_strdup(""));
	usable_length = ft_strlen (s1)
		- ft_count_first(s1, set) - ft_count_last(s1, set, total_length);
	if (usable_length == 0)
		return (ft_strdup(""));
	str = malloc(sizeof(char) * usable_length + 1);
	if (!str)
		return (0);
	return (ft_str_duplicate(s1, set, str, usable_length));
}
