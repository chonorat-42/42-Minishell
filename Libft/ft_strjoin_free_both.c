/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free_both.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 00:10:11 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/09/03 00:10:11 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

char	*strjoin_free_both(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*res;

	res = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	i = 0;
	j = 0;
	while (s1[i])
	{
		res[j] = s1[i];
		i++;
		j++;
	}
	ft_free_null(&s1);
	i = 0;
	while (s2[i])
	{
		res[j] = s2[i];
		i++;
		j++;
	}
	ft_free_null(&s2);
	res[j] = '\0';
	return (res);
}
