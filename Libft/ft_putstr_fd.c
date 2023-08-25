/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:27:59 by chonorat          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:12 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

int	ft_putstr_fd(char *s, int fd)
{
	int	index;
	int	check_error;

	check_error = 0;
	index = 0;
	if (!s)
		return (0);
	while (s[index])
	{
		check_error = write(fd, &s[index], 1);
		if (check_error == -1)
			return (-1);
		else
			index += check_error;
	}
	return (index);
}
