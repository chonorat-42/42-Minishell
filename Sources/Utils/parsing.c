/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 13:59:48 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/20 15:14:28 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

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

char *get_between_quotes(char *str, int c, size_t *i)
{
	char *result;
	int j;

	j = *i;
	if (str[j] == c)
	{
		result = ft_strdup("");
		return (result);
	}
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

void parsing(t_mshell *shell)
{
	char *temp;
	char *result;
	size_t	i;

	i = 0; 
	if (!are_all_quotes_closed(shell->prompt))
	{
		ft_printf("Error\nUnclosed Quotes\n");
		return ;
	}
	result = NULL;
	while(shell->prompt[i])
	{
		if (shell->prompt[i] == '\'' || shell->prompt[i] == '\"')
		{
			i++;
			temp = get_between_quotes(shell->prompt, shell->prompt[i], &i);
		}
		else
			temp = get_other(shell->prompt, &i);
		if (!result)
			result = ft_strdup(temp);
		else
			result = ft_strjoin(result, temp);
		free(temp);
		if (!result)
			return ;
	}
	free(shell->prompt);
	shell->prompt = ft_strdup(result);
	free(result);
}