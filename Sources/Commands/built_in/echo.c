/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:26 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/19 14:58:01 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

int are_all_quotes_closed(char *str)
{
	size_t i;
	size_t j;

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

char *get_echo_opt(char *str, size_t *i)
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
	ft_printf("echo, get_opt = %s\n\n", result);
	*i = j;
	return (result);
}

char *get_between_quotes(char *str, int c, size_t *i)
{
	char *result;
	int j;

	j = *i;
	while (str[(*i)] && str[(*i)] != c)
		(*i)++;
	result = ft_substr(str, j, *i - j);
	if (!result)
		return (NULL);
	return (result);
}

char *get_other(char *str, size_t *i)
{
	size_t j;
	char *temp;

	j = *i;
	while (str[j] && str[j] != '\'' && str[j] != '\"')
		j++;
	temp = ft_substr(str, (*i), j - (*i));
	if (!temp)
		return (NULL);
	*i = j;
	return (temp);
}

int echo_case(char *prompt, int fd)
{
	char *option;
	size_t i;
	char *to_display;
	char *temp;

	i = 5;
	option = get_echo_opt(prompt, &i);
	if (option)
	{
		if (option[0] != '-' || ft_strlen(option) != 2 || option[1] != 'n')
			return (ft_printf("Error\nWrong options for echo\n"), free(option), 0);
	}
	while (prompt[i] && is_ws(prompt[i]))
		i++;
	if (!are_all_quotes_closed(&prompt[i]))
	{
		if (option)
			free(option);
		return (ft_printf("Error\nUnclosed Quotes\n"), 0);
	}
	while (prompt[i])
	{
		if (prompt[i] == '\'' || prompt[i] == '\"')
		{
			i++;
			temp = get_between_quotes(prompt, prompt[i], &i);
		}
		else
			temp = get_other(prompt, &i);
		if (!temp)
		{
			if (option)
				free(option);
			return (1);
		}
		if (!to_display)
			to_display = ft_strdup(temp);
		else
			to_display = ft_strjoin(to_display, temp);
		free(temp);
		if (!to_display)
			return (free(option), 1);
	}
	if (to_display[0] && ft_strlen(to_display) == 0)
	{
		ft_putstr_fd(to_display, fd);
		if (option)
			write(fd, "$", 1);
		write(fd, "\n", 1);
	}
	if (option)
		free(option);
	free(to_display);
	return (0);
}
