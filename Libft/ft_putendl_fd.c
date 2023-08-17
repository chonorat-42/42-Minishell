/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:43:45 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:07 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

int	ft_putendl_fd(char *s, int fd)
{
	int	index;
	int	check_error;

	if (!s)
		return (0);
	check_error = 0;
	index = 0;
	while (s[index])
	{
		check_error = write(fd, &s[index], 1);
		if (check_error == -1)
			return (-1);
		else
			index += check_error;
	}
	index += write(fd, "\n", 1);
	return (index);
}
