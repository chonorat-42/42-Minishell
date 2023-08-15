/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:14:00 by pgouasmi          #+#    #+#             */
/*   Updated: 2022/11/29 11:41:15 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (needle[0] == '\0')
		return ((char *) haystack);
	i = 0;
	j = 0;
	while (haystack[i] && i < len)
	{
		if (haystack[i] == needle[j] && i + j < len)
		{
			while (haystack[i + j] == needle[j] && i + j < len)
			{
				j++;
				if (needle [j] == '\0')
					return ((char *) &haystack[i]);
			}
		}
		else
		{
			i++;
			j = 0;
		}
	}
	return (0);
}
