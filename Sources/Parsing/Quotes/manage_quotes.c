/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:22:52 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/26 17:22:52 by chonorat         ###   ########.fr       */
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
			if (temp)
				temp2->cmd_arr[i] = ft_strdup(temp);
			else
				temp2->cmd_arr[i] = ft_strdup("");
			free(temp);
			i++;
		}
		temp2 = temp2->next;
	}
}

void	quote_found(t_dlist **temp, char *str, size_t *i, size_t *j)
{
	(*i)++;
	*j = *i;
	while (str[(*i)] && str[(*i)] != str[*j - 1])
		(*i)++;
	split_into_dlst(temp, str, *i, *j);
	if (str[(*i)])
		(*i)++;
	*j = *i;
}

void	not_a_quote_case(char *str, t_dlist **temp, size_t *i, size_t *j)
{
	*j = *i;
	while (str[(*i)] && !is_char_in_set(str[(*i)], "\'\""))
		(*i)++;
	split_into_dlst(temp, str, *i, *j);
	*j = *i;
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
			quote_found(&temp, str, &i, &j);
		else
			not_a_quote_case(str, &temp, &i, &j);
	}
	if (temp)
	{
		result = join_dlist(temp);
		return (free_dlist(&temp), result);
	}
	return (NULL);
}
