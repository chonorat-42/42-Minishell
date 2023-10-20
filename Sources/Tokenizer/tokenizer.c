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
	(*new)->fd_in_str = NULL;
	(*new)->fd_out_str = NULL;
	(*new)->dlst = NULL;
}

void	create_token(t_mshell *shell, int i, int j, char *to_add)
{
	t_tokens	*new;
	char		*str;

	new = malloc(sizeof(t_tokens));
	str = ft_substr((const char *)to_add, j, (i - j));
	new->content = ft_strtrim(str, " \n\t\b");
	free(str);
	new->next = NULL;
	if (!shell->tok_lst)
	{
		shell->tok_lst = new;
		new->prev = NULL;
	}
	else
		tokens_addback(&shell->tok_lst, new);
	init_new_token(&new);
}

void	pipe_found(t_mshell *shell, char *str, size_t *i, size_t *j)
{
	create_token(shell, *i, *j, str);
	*j = *i;
	(*i)++;
	create_token(shell, *i, *j, str);
	*j = *i;
}

void	split_on_pipes(t_mshell *shell, char *str)
{
	size_t	i;
	size_t	j;

	i = -1;
	j = 0;
	while (str[++i])
	{
		if (i == 0 && str[i] == '|')
		{
			ft_dprintf(2, "minishell: syntax error near unexpected token `|'\n");
			return (free(shell->input), free_arr(shell->paths), shell->paths = NULL, get_input_loop(shell));
		}
		if (is_char_in_set(str[i], "\'\""))
			move_to_next_quote(str, &i, str[i]);
		else if (is_char_in_set(str[i], "|"))
		{
			if (str[i + 1] == '|')
				return (ft_free_tokens(&shell->tok_lst), free(shell->input),
					ft_dprintf(2, "Operator `||' is not supported\n"),
					get_input_loop(shell));
			else if (!str[i + 1])
			{
				ft_dprintf(2, "minishell: syntax error near unexpected token `|'\n");
				return (ft_free_tokens(&shell->tok_lst), free(shell->input), free_arr(shell->paths), shell->paths = NULL, get_input_loop(shell));
			}
			pipe_found(shell, str, &i, &j);
		}
		else
		{
			if (str[i] == '&')
				return (ft_free_tokens(&shell->tok_lst), free(shell->input),
					ft_dprintf(2, "Operator `&' and `&&' are not supported\n"),
					get_input_loop(shell));
		}
	}
	if (i != j)
		create_token(shell, i, j, str);
}

void	close_fd(t_mshell *shell)
{
	t_tokens	*temp;

	temp = shell->tok_lst;
	while (temp)
	{
		if (temp->fd_in != 0 && temp->fd_in != -1)
			close (temp->fd_in);
		if (temp->fd_out != 1 && temp->fd_out != -1)
			close(temp->fd_out);
		temp = temp->next;
	}
}

int	tokenizer(t_mshell *shell)
{
	shell->tok_lst = NULL;
	split_on_pipes(shell, shell->input);
	if (!shell->tok_lst)
		return (free(shell->input), get_input_loop(shell), 0);
	parse_tkn(&shell->tok_lst, shell);
	split_tokens_into_dlst(&shell->tok_lst, shell);

	// ft_dprintf(2, "just after split tokens into dlst, dlst =\n");
	// print_dlist(shell->tok_lst->dlst);

	get_fds(shell, &shell->tok_lst);

	// ft_dprintf(2, "after get fd\n");

	create_cmd_arr(&shell->tok_lst, shell);

	// ft_dprintf(2, "after create cmd arr\n");

	manage_quotes_arr(&shell->tok_lst);

	// ft_dprintf(2, "after manage quote arr fd\n");

	free_tokens_dlist(&shell->tok_lst);
	give_type(&shell->tok_lst);
	if (!shell->tok_lst || !shell->tok_lst->cmd_arr
		|| !shell->tok_lst->cmd_arr[0][0])
		return (close_fd(shell), ft_free_tokens(&shell->tok_lst), free(shell->input),
			get_input_loop(shell), 0);
	return (0);
}
