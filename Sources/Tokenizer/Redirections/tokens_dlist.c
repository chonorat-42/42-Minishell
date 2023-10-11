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
	while (str[(*i)] && !ft_isws(str[(*i)]) && !is_char_in_set(str[(*i)], "<>"))
		(*i)++;
	split_into_dlst(lst, str, *i, j);
}

void	get_chevrons(char *str, size_t *i, char c, t_dlist **lst)
{
	size_t	j;

	j = *i;
	while (str[(*i)] == c)
		(*i)++;
	split_into_dlst(lst, str, *i, j);
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

void	split_redir(t_mshell *shell, t_dlist **lst, char *str, size_t *i, size_t *j)
{
	if (*i != *j)
		split_into_dlst(lst, str, *i, *j);
	while (ft_isws(str[(*i)]))
		i++;
	get_chevrons(str, i, str[(*i)], lst);
	if (!str[(*i)])
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token `newline'\n");
		return (ft_free_tokens(&shell->tok_lst), get_input_loop(shell));
	}
	get_redir(str, i, lst);
	while (str[(*i)] && ft_isws(str[(*i)]))
		(*i)++;
	*j = *i;
}

void	split_words_and_redir(t_dlist **lst, char *str, t_mshell *shell)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_char_in_set(str[i], "\'\""))
		{
			move_to_next_quote(str, &i, str[i]);
			i++;
		}
		else if (is_char_in_set(str[i], "<>"))
			split_redir(shell, lst, str, &i, &j);
		else if (ft_isws(str[i]))
		{
			if (j != i)
				split_into_dlst(lst, str, i, j);
			while (str[i] && ft_isws(str[i]))
				i++;
			j = i;
		}
		else
			i++;
	}
	if (i - j)
		split_into_dlst(lst, str, i, j);
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
