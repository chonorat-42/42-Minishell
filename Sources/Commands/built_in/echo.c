/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:26 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/18 18:19:46 by pgouasmi         ###   ########.fr       */
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

char *get_echo_opt(char *str, size_t *i)
{
	size_t	j;
	char	*result;

	if (str[(*i)] != '-')
		return (NULL);
	j = *i + 1;
	while(str[j] != ' ')
		j++;
	result = ft_substr(str, (*i), j - (*i));
	if (!result)
		return ("0");
	ft_printf("echo, get_opt = %s\n\n", result);
	*i = j;
	return (result);
}

void	display_between_quotes(char *str, int c, size_t *i, int fd)
{
	// (*i)++;
	while (str[(*i)] && str[(*i)] != c)
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
	// ft_printf("echo, cmd = %s\n\n", cmd);
	i = 5;
	option = get_echo_opt(prompt, &i);
	if (option)
	{
		if (option[0] != '-' || ft_strlen(option) != 2 || option[1] != 'n')
			return (ft_printf("Error\nWrong options for echo\n"), free(cmd), free(option), 0);
	}
	while(prompt[i] && is_ws(prompt[i]))
		i++;
	// i++;
	
	
	// ft_printf("after while is_ws : prompt[i] = %c\n\n", prompt[i]);

	// ft_printf("after while is ws : &prompt[i] = %s\n\n", &prompt[i]);

	
	while (prompt[i])
	{
		if (prompt[i] != '\'' && prompt[i] != '\"')
			write(fd, &prompt[i], 1);
		else
		{

			// ft_printf("dans le else du while : prompt[i] = %c\n\n", prompt[i]);

			
			if (are_quotes_closed(&prompt[i], prompt[i]))
				display_between_quotes(&prompt[i], prompt[i], &i, fd);
			else
			{
				if (cmd)
					free(cmd);
				return(ft_printf("Error\nQuotes are not closed\n"), free(cmd), 12);
			}
		}
		if (!prompt[i + 1] && (prompt[i] == '\'' || prompt[i + 1] == '\"'))
			break;
		if (prompt[i])
			i++;
	}
	if (!option)
		write(1, "\n", 1);
	if (option)
		free(option);
	free(cmd);
	return (0);
}