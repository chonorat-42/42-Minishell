/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:30:27 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:17 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned int	index;

	index = 0;
	while (s[index])
	{
		if (s[index] == (unsigned char)c)
			return (&((char *)s)[index]);
		index++;
	}
	if (s[index] == (unsigned char)c)
		return (&((char *)s)[index]);
	return (NULL);
}
