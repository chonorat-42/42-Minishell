/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 18:13:13 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 18:24:53 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

int	are_all_quotes_closed(char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"'))
		{
			j = i + 1;
			while (str[j] && str[j] != str[i])
				j++;
			if (!str[j])
				return (0);
			i = j;
		}
		i++;
	}
	return (1);
}

void  parsing(t_mshell *shell)
{
	if (!are_all_quotes_closed(shell->input))
	{
		show_error("", "QUOTES", 1);
		return (free(shell->input), free_arr(shell->paths),
			get_input_loop(shell));
	}
	if (shell->input[0] != '\0')
	{
		if (expand(shell, shell->input) == 1)
			return (free(shell->input), get_input_loop(shell));
	}
}
