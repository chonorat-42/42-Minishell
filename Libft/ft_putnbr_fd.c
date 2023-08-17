/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:43:58 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:09 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	char	c;
	int		check_error;

	if (n == -2147483648)
		return (write(fd, "-2147483648", 11));
	if (n < 0)
	{
		n = n * -1;
		check_error = write(fd, "-", 1);
		if (check_error == -1)
			return (-1);
	}
	if (n <= 9)
	{
		c = n + '0';
		check_error = write(fd, &c, 1);
	}
	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
	return (check_error);
}
