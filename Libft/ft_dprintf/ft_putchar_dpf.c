/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_dpf.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:41:49 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/17 16:54:42 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/libft.h"

void	ft_putchar_dpf(int c, size_t *char_count, int fd)
{
	write(fd, &c, 1);
	(*char_count)++;
}
