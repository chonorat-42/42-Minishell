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

int is_ws(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

int str_is_ws(char *str)
{
	size_t i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (!is_ws(str[i]))
			return (0);
		i++;
	}
	return (1);
}

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

void print_tkns_down(t_tokens *lst)
{
	t_tokens *temp;

	if (!lst)
		return;
	temp = lst;
	if (DEBUG)
	{
		while (temp)
		{
			ft_printf("content = %s\n", temp->content);
			ft_printf("type = %d\n", temp->type);
			ft_printf("position = %d\n", temp->position);
			ft_printf("fd_in = %d\n", temp->fd_in);
			ft_printf("fd_out = %d\n\n", temp->fd_out);
			temp = temp->next;
		}
	}
}

void print_tkns_up(t_tokens *lst)
{
	t_tokens *temp;

	if (!lst)
		return;
	temp = lst;
	while (temp->next)
		temp = temp->next;
	while (temp)
	{
		ft_printf("position = %d\n", temp->position);
		ft_printf("%s\n", temp->content);
		temp = temp->prev;
	}
}

int create_token(t_mshell *shell, int i, int j, char *to_add)
{
	t_tokens *new;
	t_tokens *temp;
	char *str;
	char *result;

	str = ft_substr((const char *)to_add, j, (i - j));
	if (!str)
		return (1);
	if (!str[0])
		return (free(str), 2);
	result = ft_strtrim(str, "\n ");
	free(str);
	if (!result)
		return (1);
	if (str_is_ws(result))
		return (free(result), 2);
	new = malloc(sizeof(t_tokens));
	if (!new)
		return (free(str), free(result), 1);
	if (shell->tok_lst == NULL)
	{
		shell->tok_lst = new;
		new->position = 1;
		new->next = NULL;
		new->prev = NULL;
	}
	else
	{
		temp = shell->tok_lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
		new->prev = temp;
		new->position = temp->position + 1;
		new->next = NULL;
	}
	new->content = ft_strdup(result);
	if (new->content[0] == '|')
		new->type = PIPE;
	else
		new->type = CMD;
	new->cmd_arr = NULL;
	new->fd_in = 0;
	new->fd_out = 1;
	free(result);
	if (!new->content)
		return (1);
	return (0);
}

void	move_to_next_quote(char *str, size_t *i, char c)
{
	(*i)++;
	while (str[(*i)] && str[(*i)] != c)
		(*i)++;
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
			// i++;
		}
		else
			i++;
	}
	create_token(shell, i, j, str);
}

char	*get_next_word(char *str)
{
	size_t	i;
	size_t	j;
	char	*res;

	i = 0;
	while (str[i] && is_ws(str[i]) && is_char_in_set(str[i], "\'\""))
		i++;
	j = i;
	while (str[i] && (!is_ws(str[i]) && !is_char_in_set(str[i], "\'\"<>")))
		i++;
	res = ft_substr(str, j, i - j);
	return (res);
}

char	*get_first_word(char *str)
{
	size_t	i;
	size_t	j;
	char	*res;

	i = 0;
	while (str[i] && is_ws(str[i]))
		i++;
	j = i;
	while (str[i] && !is_ws(str[i]))
		i++;
	res = ft_substr(str, j, i - j);
	return (res);
}

void	split_into_dlst(t_dlist **lst, char *str, size_t i, size_t j)
{
	t_dlist	*new;
	t_dlist *temp;

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
		while(temp->next)
			temp = temp->next;
		temp->next = new;
		new->prev = temp;
	}
}

void	next_word_end_index(char *str, size_t *i)
{
	while (str[(*i)] && is_ws(str[(*i)]))
		(*i)++;
	while (str[(*i)] && !is_ws(str[(*i)]))
		(*i)++;
}

void	print_dlist(t_dlist	*lst)
{
	t_dlist	*temp;

	temp = lst;
	while (temp)
	{
		ft_printf("%s\n", temp->content);
		temp = temp->next;
	}
	ft_printf("\n");
}

void	remove_fd_lst(t_dlist **lst, char c, t_dlist **new)
{
	t_dlist *temp;

	temp = *lst;
	while (temp)
	{
		if (temp && temp->content[0] == c)
			temp = temp->next;
		else
		{
			split_into_dlst(new, temp->content, ft_strlen(temp->content), 0);
		}
		temp = temp->next;
	}
}

void	remove_redirect(t_tokens **lst)
{
	t_tokens	*temp;
	t_dlist		*new;

	temp = *lst;
	new = NULL;
	while (temp)
	{
		if (temp->fd_in != STDIN_FILENO)
		{
			remove_fd_lst(&temp->dlst, '<', &new);
			free_dlist(&temp->dlst);
			temp->dlst = new;
		}
		if (temp->fd_out != STDOUT_FILENO)
		{
			remove_fd_lst(&temp->dlst, '>', &new);
			free_dlist(&temp->dlst);
			temp->dlst = new;
		}
		temp = temp->next;
	}
}

size_t	dlst_size(t_dlist *lst)
{
	size_t	res;
	t_dlist	*temp;

	res = 0;
	temp = lst;
	while (temp)
	{
		res++;
		temp = temp->next;
	}
	return (res);
}

char	**list_into_arr(t_dlist *lst)
{
	char	**res;
	t_dlist	*temp;
	size_t	size;
	size_t	j;

	res = NULL;
	temp = lst;
	size = dlst_size(lst);
	res = malloc(sizeof(char *) * (size + 1));
	res[size] = NULL;
	j = 0;
	while (j < size)
	{
		res[j] = ft_strdup(temp->content);
		j++;
		temp = temp->next;
	}
	return (res);
}

void	print_arrays_in_list(t_tokens *lst)
{
	t_tokens	*temp;

	temp = lst;
	while (temp)
	{
		print_arr(temp->cmd_arr);
		ft_printf("\n");
		temp = temp->next;
	}
}

void	get_commands_lst(t_dlist *base, t_dlist **new)
{
	t_dlist	*temp;

	temp = base;
	while (temp)
	{
		if (is_char_in_set(temp->content[0], "<>"))
			temp = temp->next->next;
		else
		{
			split_into_dlst(new, temp->content, ft_strlen(temp->content), 0);
			temp = temp->next;
		}
	}
}

void	create_cmd_arr(t_tokens **tk_lst, t_mshell *shell)
{
	t_tokens	*temp;
	t_dlist		*new;

		temp = *tk_lst;
		while (temp)
		{
			new = NULL;
			get_commands_lst(temp->dlst, &new);
			temp->cmd_arr = list_into_arr(new);
			if (!temp->cmd_arr)
				return (ft_free_tokens(tk_lst), get_input_loop(shell));
			free_dlist(&temp->dlst);
			free_dlist(&new);
			temp = temp->next;
		}
}

void	print_lst_arr(t_tokens *lst)
{
	t_tokens	*temp;
	size_t		i;

	i = 1;
	temp = lst;
	while (temp)
	{
		ft_printf("array %d from \"%s\" : \n", i, temp->content);
		print_arr(temp->cmd_arr);
		ft_printf("\n");
		temp = temp->next;
		i++;
	}
}

void	free_tokens_dlist(t_tokens **lst)
{
	t_tokens	*temp;

	temp = *lst;
	while (temp)
	{
		free_dlist(&temp->dlst);
		temp = temp->next;
	}
}

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

void	get_redir(char *str, size_t *i, t_dlist **lst)
{
	size_t	j;

	while (str[(*i)] && is_ws(str[(*i)]))
		(*i)++;
	j = *i;
	while (str[(*i)] && !is_ws(str[(*i)]) && !is_char_in_set(str[(*i)],"<>"))
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
		{
			if (i != j)
				split_into_dlst(lst, str, i, j);
			while (is_ws(str[i]))
				i++;
			get_chevrons(str, &i, str[i], lst);
			if (!str[i])
				return (ft_printf("minishell: syntax error near unexpected token `newline'\n"), ft_free_tokens(&shell->tok_lst), get_input_loop(shell));
			get_redir(str, &i, lst);
			while (str[i] && is_ws(str[i]))
				i++;
			j = i;
		}
		else if (is_ws(str[i]))
		{
			if (j != i)
				split_into_dlst(lst, str, i, j);
			while(str[i] && is_ws(str[i]))
				i++;
			j = i;
		}
		else
			i++;
	}
	if (i - j)
		split_into_dlst(lst, str, i, j);
}

void	split_input_into_dlst(t_tokens **lst, t_mshell *shell)
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

void	print_tokens_dlist(t_tokens *tok)
{
	t_tokens	*temp;

	temp = tok;
	while (temp)
	{
		ft_printf("DLST FROM CONTENT %s, fd_in = %d, fd_out = %d :\n", temp->content, temp->fd_in, temp->fd_out);
		print_dlist(temp->dlst);
		temp = temp->next;
	}
}

// void	update_tok(t_mshell *shell, char *to_add)
// {
// 	size_t	i;
// 	size_t	j;

// 	i = 0;
// 	h = 0;
// 	while (str[i])
// 	{
// 		if (
// 	}
// }

void	hdoc_add_cmd(t_mshell *shell)
{
	char	*to_add;

	to_add = readline(">");
	split_on_pipes(shell, to_add);
	free(to_add);
}

int	last_is_pipe(t_tokens *tok)
{
	t_tokens	*temp;

	temp = tok;
	while (temp->next)
		temp = temp->next;
	if (temp->type == PIPE)
		return (1);
	return (0);
}

void	parse_tkn(t_tokens **tok, t_mshell *shell)
{
	t_tokens	*temp;

	temp = *tok;
	if (temp->type == PIPE)
		return (ft_printf("minishell: syntax error near unexpected token '|'\n"), ft_free_tokens(&shell->tok_lst), get_input_loop(shell));
	while(last_is_pipe(*tok))
	{
		while (temp->next)
			temp = temp->next;
		if (temp->type == PIPE)
			hdoc_add_cmd(shell);
	}
}

/*if last = pipe->heredoc 
if !redir --> error code : syntax error near unexpected token `newline'*/

int	tokenizer(t_mshell *shell)
{
	shell->tok_lst = NULL;
	split_on_pipes(shell, shell->input);
	parse_tkn(&shell->tok_lst, shell);
	split_input_into_dlst(&shell->tok_lst, shell);
	get_fds(&shell->tok_lst, shell->envp);
	create_cmd_arr(&shell->tok_lst, shell);
	manage_quotes_arr(&shell->tok_lst);
	free_tokens_dlist(&shell->tok_lst);
	give_type(&shell->tok_lst);
	return (0);
}
