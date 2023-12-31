/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:40:39 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/09 17:40:39 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_option(char *arg)
{
	int	index;

	index = 0;
	if (arg[index] != '-')
		return (0);
	index++;
	if (!arg[index])
		return (0);
	while (arg[index])
	{
		if (arg[index] != 'n')
			return (0);
		index++;
	}
	return (1);
}

static void	get_option(char **cmd, int *option, int *index)
{
	while (cmd[*index])
	{
		if (is_option(cmd[*index]) && cmd[*index][0] != '\0')
		{
			(*index)++;
			*option = 1;
		}
		else
			break ;
	}
}

void	echo_case(char **cmd, int fd)
{
	int	option;
	int	index;
	int	printed;
	int	arg_count;

	index = 1;
	printed = 0;
	option = 0;
	arg_count = ft_arr_size(cmd);
	get_option(cmd, &option, &index);
	while (index < arg_count)
	{
		if (printed)
			ft_putchar_fd(' ', fd);
		if (cmd[index])
			ft_putstr_fd(cmd[index], fd);
		printed = 1;
		index++;
	}
	if (!option)
		ft_putchar_fd('\n', fd);
	if (fd != 1)
		close(fd);
}
