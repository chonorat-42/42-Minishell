/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:43:20 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:56:57 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	index;

	index = 0;
	if (!dst && !src)
		return (NULL);
	if ((unsigned char *)src < (unsigned char *)dst)
	{
		while (len > 0)
		{
			len--;
			((unsigned char *)dst)[len] = ((unsigned char *)src)[len];
		}
	}
	else
	{
		while (index < len)
		{
			((unsigned char *)dst)[index] = ((unsigned char *)src)[index];
			index++;
		}
	}
	return ((unsigned char *)dst);
}
