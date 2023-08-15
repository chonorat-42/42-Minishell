/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_dp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:26:31 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/07/28 16:19:19 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_base_dp(size_t n, const char *base, size_t *char_count, int fd)
{
	size_t	base_length;

	base_length = ft_strlen(base);
	ft_putstr_dpf("0x", char_count, fd);
	if (n < base_length)
		ft_putchar_dpf(base[n], char_count, fd);
	else
	{
		ft_dputnbr_base(n / base_length, base, char_count, fd);
		ft_dputnbr_base(n % base_length, base, char_count, fd);
	}
}
