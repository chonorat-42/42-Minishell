/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:26 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/23 14:00:34 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_option(char **cmd, int *option, int *index)
{
	while (cmd[*index])
	{
		if (ft_strncmp(cmd[*index], "-n", ft_strlen(cmd[*index])) == 0
				&& cmd[*index][0] != '\0')
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
	arg_count = count_arr_size(cmd);
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
}
