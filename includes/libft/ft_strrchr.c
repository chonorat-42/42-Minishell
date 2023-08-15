/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 11:50:14 by pgouasmi          #+#    #+#             */
/*   Updated: 2022/11/28 17:09:48 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;

	i = ft_strlen(s);
	if ((char) c == '\0')
		return ((char *) &s[i]);
	while (i >= 0)
	{
		if (s[i] == (char) c)
			return ((char *) &s[i]);
		else
			i--;
	}
	return (0);
}
