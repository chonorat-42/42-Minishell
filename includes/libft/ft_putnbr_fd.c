/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 17:35:14 by pascal            #+#    #+#             */
/*   Updated: 2022/12/09 10:53:09 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	biggest_divider(long long int nbr)
{
	int	div;

	div = 1;
	while (nbr / div > 9)
	{
		div = div * 10;
	}
	return (div);
}

void	ft_putnbr_fd(int n, int fd)
{
	char			c;
	unsigned int	div;
	long long int	nbr;

	nbr = n;
	if (nbr < 0)
	{
		write(fd, "-", 1);
		nbr = nbr * -1;
	}
	div = biggest_divider(nbr);
	while (div >= 1)
	{
		c = (nbr / div) + 48;
		write(fd, &c, 1);
		nbr = nbr - (div * (c - 48));
		div = div / 10;
	}
}
