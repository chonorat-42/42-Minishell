/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 18:53:27 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/20 18:58:25 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while(s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++; 
	}
	if (s1[i] || s2[i])
		return (1);
	return (0);
}
