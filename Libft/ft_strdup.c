/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:31:03 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:19 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

char	*ft_strdup(const char *s1)
{
	unsigned int	index;
	char			*malloc_s1;

	malloc_s1 = (char *)malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (!malloc_s1)
		return (NULL);
	index = 0;
	while (s1[index])
	{
		malloc_s1[index] = s1[index];
		index++;
	}
	malloc_s1[index] = '\0';
	return (malloc_s1);
}
