/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:26 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/20 18:47:40 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

char *get_builtin_opt(char *str, size_t *i)
{
	size_t j;
	char *result;

	if (str[(*i)] != '-')
		return (NULL);
	j = *i + 1;
	while (str[j] != ' ')
		j++;
	result = ft_substr(str, (*i), j - (*i));
	if (!result)
		return ("0");
	*i = j;
	return (result);
}

int echo_case(char *prompt, int fd)
{
	char *option;
	size_t i;

	i = 5;
	option = get_builtin_opt(prompt, &i);
	if (option)
	{
		if (ft_strncmp(option, "-n", ft_strlen(option)))
			return (ft_printf("Error\nWrong options for echo\n"), free(option), 0);
	}
	while (prompt[i] && is_ws(prompt[i]))
		i++;
	while(prompt[i])
	{
		write(fd, &prompt[i], 1);
		i++;
	}
	if (option)
	{
		ft_dprintf(fd, "$");
		free(option);
	}
	ft_dprintf(fd, "\n");
	return (0);
}
