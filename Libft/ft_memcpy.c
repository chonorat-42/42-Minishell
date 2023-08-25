/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:43:11 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:56:55 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

void	*ft_memcpy(void *restrict dst, const void *restrict src, size_t n)
{
	size_t			index;
	unsigned char	*unsigned_dst;
	unsigned char	*unsigned_src;

	unsigned_dst = (unsigned char *)dst;
	unsigned_src = (unsigned char *)src;
	index = 0;
	if (dst || src)
	{
		while (index < n)
		{
			unsigned_dst[index] = unsigned_src[index];
			index++;
		}
		return (dst);
	}
	return (NULL);
}
