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

void	manage_quotes_arr(t_tokens	**lst)
{
	size_t		i;
	t_tokens	*temp2;
	char		*temp;

	temp2 = *lst;
	while (temp2)
	{
		i = 0;
		while (temp2->cmd_arr[i])
		{
			temp = remove_quotes(temp2->cmd_arr[i]);
			free(temp2->cmd_arr[i]);
			temp2->cmd_arr[i] = ft_strdup(temp);
			free(temp);
			i++;
		}
		temp2 = temp2->next;
	}
}

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

char	*remove_quotes(char *str)
{
	t_dlist	*temp;
	char	*result;
	size_t	i;
	size_t	j;

	temp = NULL;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_char_in_set(str[i], "\'\""))
		{
			j = i + 1;
			move_to_next_quote(str, &i, str[i]);
			split_into_dlst(&temp, str, i, j);
			i++;
			j = i;
		}
		else
		{
			while (str[i] && !is_char_in_set(str[i], "\'\""))
				i++;
			split_into_dlst(&temp, str, i, j);
		}
	}
	result = join_dlist(temp);
	return (free_dlist(&temp), result);
}

// char	*remove_quotes(char	*str)
// {
// 	char	*temp;
// 	char	*result;
// 	size_t	i;

// 	i = 0; 
// 	result = NULL;
// 	while(str[i])
// 	{
// 		if (str[i] == '\'' || str[i] == '\"')
// 		{
// 			i++;
// 			temp = get_between_quotes(str, str[i - 1], &i);
// 			i++;
// 		}
// 		else
// 			temp = get_other(str, &i);
// 		if (!result)
// 			result = ft_strdup(temp);
// 		else
// 			result = ft_strjoin(result, temp);
// 		free(temp);
// 		if (!result)
// 			return (NULL);
// 	}
// 	return (result);
// }
