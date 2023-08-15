/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dputnbr_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:39:24 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/07/28 15:57:29 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dputnbr_base(long long int n, const char *base, size_t *char_count, int fd)
{
	size_t	nbr;
	size_t	base_length;

	base_length = ft_strlen(base);
	if (n < 0)
	{
		ft_putchar_dpf('-', char_count, fd);
		nbr = n * -1;
	}
	else
		nbr = n;
	if (nbr < base_length)
		ft_putchar_dpf(base[nbr], char_count, fd);
	else
	{
		ft_dputnbr_base(nbr / base_length, base, char_count, fd);
		ft_dputnbr_base(nbr % base_length, base, char_count, fd);
	}
}
