/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:31:24 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:24 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char				*new_str;
	unsigned int		index;
	unsigned int		index_j;

	index = 0;
	index_j = 0;
	if (!s1 || !s2)
		return (NULL);
	new_str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!new_str)
		return (NULL);
	while (s1[index])
	{
		new_str[index] = s1[index];
		index++;
	}
	while (s2[index_j])
		new_str[index++] = s2[index_j++];
	new_str[index] = '\0';
	return (new_str);
}
