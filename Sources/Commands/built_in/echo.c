/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:26 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/18 15:34:20 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

int are_quotes_closed(char *str, int c)
{
	size_t i = 0;

	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char *get_cmd_arguments(char *prompt)
{
	char *result;
	size_t i = 0;
	size_t total_len = ft_strlen(prompt);
	size_t res_len;
	size_t j;

	while (prompt[i] && prompt[i] != ' ')
		i++;
	i++;
	res_len = total_len - i;
	result = malloc(sizeof(char) * (res_len) + 2);
	j = 0;
	while (prompt[i])
	{
		result[j] = prompt[i];
		i++;
		j++;
	}
	result[j] = '\n';
	result[j + 1] = '\0';
	return (result);
}

char *get_echo_opt(char *str, size_t *i)
{
	size_t	j;
	char	*result;

	if (str[(*i)] != '-')
		return (NULL);
	j = *i;
	while(!is_ws(str[j]))
		j++;
	result = ft_substr(str, (*i), j - 1);
	if (!result)
		return ("0");
	ft_printf("echo, get_opt = %s\n\n", result);
	return (result);
}

void	display_between_quotes(char *str, int c, size_t *i, int fd)
{
	while (str[(*i)] != c)
	{
		write(fd, &str[(*i)], 1);
		(*i)++;
	}
}

int echo_case(char *prompt, int fd)
{
	char *cmd;
	char *option;
	size_t	i;

	cmd = ft_substr(prompt, 0, 4);
	ft_printf("echo, cmd = %s\n\n", cmd);
	i = 5;
	option = get_echo_opt(prompt, &i);
	if (option)
	{
		if (option[0] != '-' || ft_strlen(option) != 2 || option[1] != 'n')
			return (free(cmd), free(option), 1);
	}
	while (prompt[i])
	{
		if (prompt[i] != '\'' && prompt[i] != '\"')
			write(fd, &prompt[i], 1);
		else
		{
			if (are_quotes_closed(&prompt[i], prompt[i]))
				display_between_quotes(&prompt[i + 1], prompt[i], &i, fd);
		}
		i++;
	}
	if (!option)
		write(1, "\n", 1);
	if (option)
		free(option);
	free(cmd);
	return (0);
}