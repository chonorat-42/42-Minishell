/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 12:19:09 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:47 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*pointer;
	size_t	s_len;
	size_t	index;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	s_len = ft_strlen(s + start);
	if (len > s_len)
		len = s_len;
	pointer = (char *)malloc((sizeof(char) * len) + 1);
	if (!pointer)
		return (NULL);
	index = 0;
	while (index < len)
		pointer[index++] = s[start++];
	pointer[index] = '\0';
	return (pointer);
}
