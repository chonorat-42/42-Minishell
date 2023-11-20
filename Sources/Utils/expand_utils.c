/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:03:32 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/06 18:03:32 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_between_simple(char *str)
{
	size_t	i;
	size_t	s_quote_count;

	i = 0;
	s_quote_count = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			move_to_last_quote(str, &i, str[i]);
		if (str[i] == '\'')
			s_quote_count++;
		if ((str[i]) == '$')
			return (s_quote_count % 2);
		i++;
	}
	return (0);
}

void	add_flag_expand(t_dlist *lst, int to_expand)
{
	t_dlist	*temp;

	temp = lst;
	if (temp)
	{
		while (temp->next)
			temp = temp->next;
		temp->exp = to_expand;
	}
}

void	envvar_flag(size_t index[2], size_t qcount[2], t_dlist **lst, char *str)
{
	if (index[1] != index[0])
		split_into_dlst(lst, str, index[0], index[1]);
	add_flag_expand(*lst, 1);
	index[1] = index[0];
	index[0] += last_envvar_char(&str[index[0]]);
	split_into_dlst(lst, str, index[0], index[1]);
	if (qcount[1] % 2)
		add_flag_expand(*lst, 1);
	else
		add_flag_expand(*lst, (qcount[0] % 2) + 1);
	index[1] = index[0];
}

int	is_char_in_set(char c, char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

size_t	last_envvar_char(char *str)
{
	size_t	i;

	i = 1;
	if (str[i] && str[i] == '?')
		return (i + 1);
	while (str[i])
	{
		if (str[i] == '_')
			i++;
		else if (!ft_isalnum(str[i]) || is_char_in_set(str[i], "?"))
			return (i);
		else
			i++;
	}
	return (i);
}
