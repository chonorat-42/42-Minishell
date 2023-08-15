/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_dpf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:40:52 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/07/28 16:01:40 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_dpf(const char *s, size_t *char_count, int fd)
{
	if (!s)
		ft_putstr_dpf((const char *)"(null)", char_count, fd);
	else
	{
		while (*s)
		{
			ft_putchar_dpf(*s, char_count, fd);
			s++;
		}
	}
}
