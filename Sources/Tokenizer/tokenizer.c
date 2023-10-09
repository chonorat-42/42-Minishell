/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:58:25 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 13:45:28 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	give_type(t_tokens **lst)
{
	t_tokens	*temp;

	temp = *lst;
	while (temp)
	{
		if (temp->content[0] == '|')
			temp->type = PIPE;
		else
			temp->type = CMD;
		temp = temp->next;
	}
}

void	tokens_addback(t_tokens **lst, t_tokens *new)
{
	t_tokens	*temp;

	temp = *lst;
	while (temp->next)
		temp = temp->next;
	new->next = NULL;
	new->prev = temp;
	temp->next = new;
}

void	init_new_token(t_tokens **new)
{
	(*new)->cmd_arr = NULL;
	if ((*new)->content[0] == '|')
		(*new)->type = PIPE;
	else
		(*new)->type = CMD;
	(*new)->fd_in = 0;
	(*new)->fd_out = 1;
}

void create_token(t_mshell *shell, int i, int j, char *to_add)
{
	t_tokens *new;
	char *str;

	new = malloc(sizeof(t_tokens));
	str = ft_substr((const char *)to_add, j, (i - j));
	new->content = ft_strtrim(str, " \n\t\b");
	free(str);
	new->next = NULL;
	if (!shell->tok_lst)
		shell->tok_lst = new;
	else
		tokens_addback(&shell->tok_lst, new);
	init_new_token(&new);
}

void	split_on_pipes(t_mshell *shell, char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_char_in_set(str[i], "\'\""))
			move_to_next_quote(str, &i, str[i]);
		else if (is_char_in_set(str[i], "|"))
		{
			create_token(shell, i, j, str);
			j = i;
			i++;
			create_token(shell, i, j, str);
			j = i;
		}
		else
			i++;
	}
	create_token(shell, i, j, str);
}

int	tokenizer(t_mshell *shell)
{
	shell->tok_lst = NULL;
	split_on_pipes(shell, shell->input);
	parse_tkn(&shell->tok_lst, shell);
	split_tokens_into_dlst(&shell->tok_lst, shell);
	get_fds(shell, &shell->tok_lst);
	create_cmd_arr(&shell->tok_lst, shell);
	manage_quotes_arr(&shell->tok_lst);
	free_tokens_dlist(&shell->tok_lst);
	give_type(&shell->tok_lst);
	return (0);
}
