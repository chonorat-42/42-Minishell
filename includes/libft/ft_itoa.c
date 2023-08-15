/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 15:19:03 by pgouasmi          #+#    #+#             */
/*   Updated: 2022/12/09 10:52:39 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_digit_count(unsigned int n, int *div)
{
	int	i;

	i = 1;
	while (n / *div > 9)
	{
		*div = *div * 10;
		i++;
	}
	return (i);
}

static char	*ft_put_digit(char *str, unsigned int nbr, int i, int diviseur)
{
	while (diviseur >= 1)
	{
		str[i] = nbr / diviseur + 48;
		nbr = nbr - ((str[i] - 48) * diviseur);
		diviseur = diviseur / 10;
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	char				*str;
	int					i;
	int					div;
	unsigned int		nbr;

	i = 0;
	div = 1;
	nbr = n;
	if (n < 0)
	{
		nbr = nbr * -1;
		str = malloc(sizeof(char) * ft_digit_count(nbr, &div) + 2);
	}
	else
		str = malloc(sizeof(char) * ft_digit_count(nbr, &div) + 1);
	if (!str)
		return (0);
	if (n < 0)
	{
		str[i] = '-';
		nbr = n * -1;
		i++;
	}
	return (ft_put_digit(str, nbr, i, div));
}
