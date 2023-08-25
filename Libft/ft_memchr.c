/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:42:50 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:56:48 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			index;
	unsigned char	unsigned_c;
	unsigned char	*unsigned_s;

	unsigned_c = (unsigned char)c;
	unsigned_s = (unsigned char *)s;
	index = 0;
	while (index < n)
	{
		if (unsigned_s[index] == unsigned_c)
			return (&unsigned_s[index]);
		index++;
	}
	return (NULL);
}
