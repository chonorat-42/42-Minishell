/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:33:26 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:32 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	char_count;

	if (!s)
		return (0);
	char_count = 0;
	while (s[char_count])
		char_count++;
	return (char_count);
}
