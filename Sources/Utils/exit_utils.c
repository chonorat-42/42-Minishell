/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 23:31:39 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/25 23:52:44 by chonorat         ###   ########.fr       */
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

char	*get_exit(char *cmd)
{
	int		index;
	char	*exit;

	index = 5;
	exit = NULL;
	while (cmd[index] && cmd[index] == '0')
		index++;
	if (cmd[index])
		exit = ft_strdup(&cmd[index]);
	return (exit);
}

int	only_digit(char *cmd)
{
	int	index;

	index = 0;
	if (cmd[index] == '-')
		index++;
	while (cmd[index])
	{
		if (!ft_isdigit(cmd[index]))
			return (free(cmd), cmd = NULL, 0);
		index++;
	}
	if ((ft_strlen(cmd) > 20))
		return (free(cmd), cmd = NULL, 0);
	if (ft_strlen(cmd) == 19)
		if (ft_strncmp(cmd, MAX_LL, 19) > 0)
			return (free(cmd), cmd = NULL, 0);
	if (ft_strlen(cmd) == 20)
		if (ft_strncmp(cmd, MIN_LL, 20) > 0)
			return (free(cmd), cmd = NULL, 0);
	return (free(cmd), cmd = NULL, 1);
}