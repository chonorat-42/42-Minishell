/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 13:18:46 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/07/26 15:47:07 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	line_count(char const *s, char c)
{
	size_t	i;	
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			j++;
		i++;
	}
	return (j);
}

static size_t	ft_word_length(char const *s, char c, size_t i)
{
	size_t	j;

	j = 0;
	while (s[i] != c && s[i])
	{
		i++;
		j++;
	}
	return (j);
}

static void	ft_free(char **tab)
{
	size_t	j;

	j = 0;
	while (tab[j])
	{
		free(tab[j]);
		j++;
	}
	free(tab);
}

static char	**fill_tab(size_t word_count, const char *s, char **tab, char c)
{
	size_t	i;
	size_t	j;
	size_t	word_length;

	i = 0;
	j = 0;
	while (j < word_count)
	{
		while (s[i] == c)
			i++;
		word_length = ft_word_length(s, c, i);
		tab[j] = ft_substr(s, i, word_length);
		if (!tab[j])
		{
			ft_free(tab);
			return (0);
		}
		i = i + word_length;
		while (s[i] == c && s[i])
			i++;
		j++;
	}
	tab[j] = NULL;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char			**tab;
	size_t			word_count;

	word_count = line_count(s, c);
	tab = (char **) malloc (sizeof(char *) * (word_count + 1));
	if (!tab)
		return (0);
	return (fill_tab(word_count, s, tab, c));
}
