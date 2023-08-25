/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:43:30 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:01 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			index;
	unsigned char	*unsigned_b;

	unsigned_b = (unsigned char *)b;
	index = 0;
	while (index < len)
	{
		unsigned_b[index] = c;
		index++;
	}
	return (unsigned_b);
}
