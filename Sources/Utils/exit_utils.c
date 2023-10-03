/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 23:31:39 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/03 13:09:07 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_space(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

static int	check_number(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long long	ll_atoi(const char *str)
{
	long long	value;
	int			index;
	int			sign;

	value = 0;
	index = 0;
	sign = 1;
	while (check_space(str[index]))
		index++;
	if (str[index] == '-' || str[index] == '+')
	{
		if (str[index] == '-')
			sign *= -1;
		index++;
	}
	while (check_number(str[index]))
	{
		value = (value * 10) + (str[index] - '0');
		index++;
	}
	return (value * sign);
}

static int	get_exit(char *arg, int *index)
{
	int	negative;

	negative = 0;
	if (arg[*index] == '-')
	{
		negative = 1;
		(*index)++;
	}
	else if (arg[*index] == '+')
		(*index)++;
	while (arg[*index + 1] && arg[*index] == '0')
		(*index)++;
	return (negative);
}

int	only_digit(char *arg)
{
	int	index;
	int	negative;
	int	value;

	index = 0;
	negative = get_exit(arg, &index);
	value = index;
	while (arg[index])
	{
		if (!ft_isdigit(arg[index]))
			return (0);
		index++;
	}
	if ((ft_strlen(&arg[value]) >= 20))
		return (0);
	if (ft_strlen(&arg[value]) == 19)
	{
		if (ft_strncmp(&arg[value], MAX_LL, 19) > 0 && !negative)
			return (0);
		else if (ft_strncmp(&arg[value], MIN_LL, 19) > 0 && negative)
			return (0);
	}
	return (1);
}
