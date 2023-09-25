/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:59:01 by chonorat          #+#    #+#             */
/*   Updated: 2023/09/25 18:05:03 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_status;

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

unsigned long long	ull_atoi(const char *str)
{
	unsigned long long	value;
	int					index;
	int					sign;

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

static int	only_digit(char *cmd)
{
	int	index;

	index = 5;
	if (cmd[index] == '-')
		index++;
	while (cmd[index])
	{
		if (!ft_isdigit(cmd[index]))
			return (0);
		index++;
	}
	if ((ft_strlen(&cmd[5]) > 20))
		return (0);
	if (ft_strlen(&cmd[5]) == 19)
		if (ft_strncmp(&cmd[5], MAX_LL, 19) > 0)
			return (0);
	if (ft_strlen(&cmd[5]) == 20)
		if (ft_strncmp(&cmd[5], MIN_LL, 20) > 0)
			return (0);
	return (1);
}

void	exit_case(t_mshell *shell, char *cmd)
{
	long long	exit_value;
	char		*value_str;

	ft_putendl_fd("exit", 1);
	if (ft_strlen(cmd) > 5)
	{
		if (!only_digit(cmd))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", &cmd[5]);
			exit_value = 2;
		}
		else
		{
			value_str = ft_strdup(&cmd[5]);
			exit_value = ull_atoi(value_str);
			free(value_str);
		}
	}
	else
		exit_value = shell->exit_status;
	exit(exit_value % 256);
}