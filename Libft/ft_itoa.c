/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 14:33:41 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:56:22 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

static long int	count_int(int n)
{
	long int	count;

	count = 1;
	while (n > 9 || n < -9)
	{
		count++;
		n /= 10;
	}
	if (n < 0)
		count++;
	return (count);
}

char	*ft_itoa(int n)
{
	unsigned int	lenght;
	long int		number;
	char			*char_n;

	number = n;
	lenght = count_int(n);
	char_n = (char *)malloc((lenght + 1) * sizeof(char));
	if (!char_n)
		return (NULL);
	char_n[lenght] = '\0';
	if (number < 0)
	{
		char_n[0] = '-';
		number *= -1;
	}
	while (number > 0)
	{
		lenght--;
		char_n[lenght] = (number % 10) + '0';
		number /= 10;
	}
	if (n == 0)
		char_n[0] = '0';
	return (char_n);
}
