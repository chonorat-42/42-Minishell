/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:32:46 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:26 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

size_t	ft_strlcat(char *restrict dst, const char *restrict src, size_t dstsize)
{
	size_t	index;
	size_t	index_j;

	index = 0;
	index_j = 0;
	if ((!dst || !src) && dstsize == 0)
		return (0);
	while (dst[index] && index < dstsize)
		index++;
	if (index < dstsize)
	{
		while (src[index_j] && index < (dstsize - 1))
			dst[index++] = src[index_j++];
		dst[index] = '\0';
	}
	while (src[index_j])
	{
		index++;
		index_j++;
	}
	return (index);
}
