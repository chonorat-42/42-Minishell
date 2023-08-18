/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:26 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/18 11:40:11 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

int are_quotes_closed(char *str)
{
	size_t len = ft_strlen(str);
	size_t i = 0;
	size_t j = len;

	while (str[i] && i != j)
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			while (str[j] != '\'' && str[j] != '\"' && j > 0)
				j--;
			if (str[i] != str[j] || j == i)
				return (0);
		}
		i++;
	}
	return (1);
}

int echo_case(char *prompt, int fd)
{
	char *str;
	char *temp;

	str = get_cmd_arguments(prompt);
	if (!str)
		return (1);
	are_quotes_closed(str);

	size_t i = 0;
	while (!are_quotes_closed(str))
	{
		i++;
		ft_putstr_fd("> ", fd);
		char *line = get_next_line(1);
		if (!line)
			return (free(str), 1);
		temp = ft_strjoin(str, line);
		if (!temp)
			return (free(line), free(str), 2);
		free(str);
		str = ft_strdup(temp);
		free(line);
		free(temp);
	}
	char *result;
	result = ft_strtrim(str, "\"\n\'");
	if (!result)
		return (3);
	ft_dprintf(fd, "%s\n", result);
	free(str);
	free(result);
	return (0);
}