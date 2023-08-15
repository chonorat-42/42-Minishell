/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 14:05:35 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/04/13 10:30:47 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_pos_neg(const char *str, int *i)
{
	if (str[(*i)] == '-')
	{
		(*i)++;
		return (0);
	}
	else if (str[(*i)] == '+')
	{
		(*i)++;
		return (1);
	}
	else if (str[(*i)] >= '0' && str[(*i)] <= '9')
		return (1);
	else
		return (2);
}

static void	ft_skipws(const char *str, int *i)
{
	while (str[(*i)] == '\t' || str[(*i)] == '\n'
		|| str[(*i)] == '\v' || str[(*i)] == '\f'
		|| str[(*i)] == '\r' || str[(*i)] == ' ')
		(*i)++;
}

int	ft_atoi(const char *str)
{
	long long int	result;
	int				i;
	int				sign;

	i = 0;
	result = 0;
	ft_skipws(str, &i);
	sign = check_pos_neg(str, &i);
	if (sign == 2)
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	if (sign == 0)
		result = result * -1;
	if (result > 2147483647 || result < -2147483648)
		return (0);
	return ((int)result);
}
