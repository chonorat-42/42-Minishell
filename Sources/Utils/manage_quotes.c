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

char	*get_other(char *str, size_t *i)
{
	size_t	j;
	char	*temp;

	j = *i + 1;
	while (str[j] && str[j] != '\'' && str[j] != '\"')
		j++;
	temp = ft_substr(str, (*i), j - (*i));
	if (!temp)
		return (NULL);
	*i = j;
	return (temp);
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

char	*remove_quotes(char	*str)
{
	char	*temp;
	char	*result;
	size_t	i;

	i = 0; 
	result = NULL;
	while(str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			i++;
			temp = get_between_quotes(str, str[i - 1], &i);
			i++;
		}
		else
			temp = get_other(str, &i);
		if (!result)
			result = ft_strdup(temp);
		else
			result = ft_strjoin(result, temp);
		free(temp);
		if (!result)
			return (NULL);
	}
	return (result);
}

void	manage_quotes(t_tokens **lst)
{
	t_tokens	*tmp;
	char		*new_content;

	tmp = *lst;
	while (tmp)
	{
		new_content = remove_quotes(tmp->content);
		free(tmp->content);
		tmp->content = ft_strdup(new_content);
		free(new_content);
		tmp = tmp->next;
	}
}