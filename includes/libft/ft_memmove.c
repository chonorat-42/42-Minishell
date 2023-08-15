/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:09:28 by pgouasmi          #+#    #+#             */
/*   Updated: 2022/11/28 14:47:39 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*s;

	if (!src && !dst)
		return (0);
	s = (unsigned char *) dst;
	if (dst > src)
	{
		i = len;
		while (i--)
			s[i] = ((unsigned char *)src)[i];
	}
	else
		i = 0;
	while (i < len)
	{
		s[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}
