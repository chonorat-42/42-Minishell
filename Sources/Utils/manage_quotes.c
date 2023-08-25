/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 13:59:48 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 18:09:57 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void manage_quotes(t_mshell *shell)
{
	char *temp;
	char *result;
	size_t	i;

	i = 0; 
	if (!are_all_quotes_closed(shell->input))
	{
		ft_printf("Error\nUnclosed Quotes\n");
		return ;
	}
	result = NULL;
	while(shell->input[i])
	{
		if (shell->input[i] == '\'' || shell->input[i] == '\"')
		{
			i++;
			temp = get_between_quotes(shell->input, shell->input[i], &i);
		}
		else
			temp = get_other(shell->input, &i);
		if (!result)
			result = ft_strdup(temp);
		else
			result = ft_strjoin(result, temp);
		free(temp);
		if (!result)
			return ;
	}
	free(shell->input);
	shell->input = ft_strdup(result);
	free(result);
}