/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 15:43:03 by pgouasmi          #+#    #+#             */
/*   Updated: 2022/12/09 10:54:41 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	str_length;
	char	*str;

	if (!s)
		return (0);
	str_length = ft_strlen(s);
	if (start >= str_length)
		return (ft_strdup(""));
	if (len > (str_length - start))
		len = str_length - start;
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (0);
	i = start;
	j = 0;
	while (s[i] != '\0' && j < len)
	{
		str[j] = s[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (str);
}
