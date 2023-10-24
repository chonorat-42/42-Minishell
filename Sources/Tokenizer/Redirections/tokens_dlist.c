/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_dlist.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:11:05 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/06 19:11:05 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_redir(char *str, size_t *i, t_dlist **lst)
{
	size_t	j;

	while (str[(*i)] && ft_isws(str[(*i)]))
		(*i)++;
	j = *i;
	if (is_char_in_set(str[(*i)], "\'\""))
	{
		while (is_char_in_set(str[(*i)], "\'\""))
		{
			move_to_next_quote(str, i, str[j]);
			if (ft_isws(str[(*i + 1)]) || !str[(*i + 1)])
				split_into_dlst(lst, str, ++(*i), j);
			else
				(*i)++;
		}
	}
	else
	{
		while (str[(*i)] && !ft_isws(str[(*i)])
			&& !is_char_in_set(str[(*i)], "<>"))
			(*i)++;
		split_into_dlst(lst, str, *i, j);
	}
}

void	split_into_dlst(t_dlist **lst, char *str, size_t i, size_t j)
{
	t_dlist	*new;
	t_dlist	*temp;

	new = malloc(sizeof(t_dlist));
	new->next = NULL;
	new->content = ft_substr(str, j, i - j);
	if (!*lst)
	{
		*lst = new;
		new->prev = NULL;
	}
	else
	{
		temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
		new->prev = temp;
	}
}

void	split_redir(t_mshell *shell, t_dlist **lst, char *str, size_t *index)
{
	if (index[0] != index[1])
		split_into_dlst(lst, str, index[0], index[1]);
	while (ft_isws(str[index[0]]))
		index[0]++;
	get_chevrons(str, &index[0], str[index[0]], lst);
	if (!str[index[0]])
	{
		show_error("newline", SYNTAX, 0);
		return (free_tokens(&shell->tok_lst), free_arr(shell->paths),
			shell->paths = NULL, get_input_loop(shell));
	}
	get_redir(str, &index[0], lst);
	while (str[index[0]] && ft_isws(str[index[0]]))
		index[0]++;
	index[1] = index[0];
}

void	split_words_and_redir(t_dlist **lst, char *str, t_mshell *shell)
{
	size_t	index[2];

	index[0] = 0;
	index[1] = 0;
	while (str[index[0]])
	{
		if (is_char_in_set(str[index[0]], "\'\""))
			move_to_last_quote(str, &index[0], str[index[0]++]);
		else if (is_char_in_set(str[index[0]], "<>"))
			split_redir(shell, lst, str, index);
		else if (ft_isws(str[index[0]]))
		{
			if (index[1] != index[0])
				split_into_dlst(lst, str, index[0], index[1]);
			while (str[index[0]] && ft_isws(str[index[0]]))
				index[0]++;
			index[1] = index[0];
		}
		else
			index[0]++;
	}
	if (index[0] - index[1])
		split_into_dlst(lst, str, index[0], index[1]);
}

void	split_tokens_into_dlst(t_tokens **lst, t_mshell *shell)
{
	t_tokens	*temp;

	temp = *lst;
	while (temp)
	{
		temp->dlst = NULL;
		split_words_and_redir(&temp->dlst, temp->content, shell);
		temp = temp->next;
	}
}
