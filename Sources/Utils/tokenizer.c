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

int lst_type_missing(t_tokens **lst)
{
	t_tokens *temp;

	temp = *lst;
	while (temp)
	{
		if (temp->type == 0)
			return (1);
		temp = temp->next;
	}
	return (0);
}

int is_ws(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

size_t find_space_index(char *str, int count)
{
	int res;
	size_t i;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (is_ws(str[i]))
			res++;
		if (res == count)
			return (i);
		i++;
	}
	return (0);
}

int word_count(char *str)
{
	size_t i;
	size_t result;

	i = 0;
	result = 0;
	while (str[i])
	{
		while (str[i] && is_ws(str[i]))
			i++;
		if (!is_ws(str[i]))
		{
			result++;
			while (str[i] && !is_ws(str[i]))
				i++;
		}
	}
	return (result);
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

size_t next_quote(char *str, int start, char c)
{
	size_t i;

	i = start + 1;
	while (str[i] && str[i] != c)
		i++;
	return (i);
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

int create_token(t_mshell *shell, int i, int j)
{
	t_tokens *new;
	t_tokens *temp;
	char *str;
	char *result;

	str = ft_substr((const char *)shell->input, j, (i - j));
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
	new->type = 0;
	new->cmd_arr = NULL;
	free(result);
	if (!new->content)
		return (1);
	return (0);
}

int is_sep(char c)
{
	size_t i;

	i = 0;
	while ("<>|"[i])
	{
		if ("<>|"[i] == c)
			return (1);
		i++;
	}
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
	char	c;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_char_in_set(str[i], "\'\""))
		{
			c = str[i];
			move_to_next_quote(str, &i, c);
		}
		else if (is_char_in_set(str[i], "|"))
		{
			create_token(shell, i, j);
			j = i;
			i++;
			create_token(shell, i, j);
			j = i;
			i++;
		}
		else
			i++;
	}
	create_token(shell, i, j);
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

void	get_fd_out(t_tokens **tok)
{
	size_t	i;
	size_t	j;
	char	c;
	char	*redir;
	char	*outfile;

	i = 0;
	redir = NULL;
	outfile = NULL;
	while ((*tok)->content[i])
	{
		if (is_char_in_set((*tok)->content[i], "\'\""))
		{
			c = (*tok)->content[i];
			move_to_next_quote((*tok)->content, &i, c);
		}
		else if ((*tok)->content[i] == '>')
		{
			j = i;
			while ((*tok)->content[i] == '>')
				i++;
			redir = ft_substr((*tok)->content, j, i - j);
			j = i;
			while(is_ws((*tok)->content[i]))
				i++;
			outfile = get_next_word(&(*tok)->content[i]);
		}
		i++;
	}
	if (outfile && !ft_strcmp(redir, ">"))
		(*tok)->fd_out = open(outfile, O_CREAT | O_RDWR, 0666);
	
	else if (outfile && !ft_strcmp(redir, ">>"))
		(*tok)->fd_out = open(outfile, O_APPEND | O_RDWR, 0666);

	else
		(*tok)->fd_out = 1;
	if (redir)
		free(redir);
	if (outfile)
		free(outfile);
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

void	delete_heredoc(t_tokens **tok)
{
	size_t	i;
	size_t	j;
	char	*before;
	char	*after;
	char	*result;

	i = 0;
	j = 0;
	while ((*tok)->content[i])
	{
		if (is_char_in_set((*tok)->content[i], "\'\""))
			move_to_next_quote((*tok)->content, &i, (*tok)->content[i]);
		else if ((*tok)->content[i] == '<' && (*tok)->content[i + 1] == '<')
		{
			if (i == 0)
				before = ft_strdup("");
			else
				before = ft_substr((*tok)->content, j, i - j);
			while ((*tok)->content[i] == '<')
				i++;
			while (is_ws((*tok)->content[i]))
				i++;
			while ((*tok)->content[i] && !is_ws((*tok)->content[i]) && !is_char_in_set((*tok)->content[i], "<>\'\""))
				i++;
			if (!(*tok)->content[i])
				after = ft_strdup("");
			else
				after = ft_substr((*tok)->content, i, ft_strlen((*tok)->content));
		}
		else
			i++;
	}
	if (before[0] && after[0])
	{
		result = strjoin_free_first(before, " ");
		result = strjoin_free_both(before, after);
		free((*tok)->content);
		(*tok)->content = ft_strdup(result);
		free(result);
		return ;
	}
	else if (!before[0] && after[0])
	{
		free((*tok)->content);
		(*tok)->content = ft_strdup(after);
	}
	else
	{
		free((*tok)->content);
		(*tok)->content = ft_strdup(before);
	}
}

void	expand_into_heredoc(t_tokens **tok)
{
	char	*heredoc;

	heredoc = ft_strdup("< /tmp/temp.heredoc");
	delete_heredoc(tok);
	(*tok)->content = strjoin_free_first((*tok)->content, " ");
	(*tok)->content = strjoin_free_both((*tok)->content, heredoc);
}

void	get_fd_in(t_tokens **tok, t_envp *envp)
{
	size_t	i;
	size_t	j;
	char	c;
	char	*redir;
	char	*infile;

	i = 0;
	redir = NULL;
	infile = NULL;
	while ((*tok)->content[i])
	{
		if (is_char_in_set((*tok)->content[i], "\'\""))
		{
			c = (*tok)->content[i];
			move_to_next_quote((*tok)->content, &i, c);
		}
		else if ((*tok)->content[i] == '<')
		{
			j = i;
			while ((*tok)->content[i] == '<')
				i++;
			redir = ft_substr((*tok)->content, j, i - j);
			j = i;
			while(is_ws((*tok)->content[i]))
				i++;
			infile = get_next_word(&(*tok)->content[i]);
		}
		i++;
	}
	if (infile && !ft_strcmp(redir, "<"))
		(*tok)->fd_in = open(infile, O_RDWR);
	else if (infile && !ft_strcmp(redir, "<<"))
	{
		(*tok)->fd_in = open("/tmp/temp.heredoc", O_RDWR | O_CREAT, 0666);
		heredoc(infile, (*tok)->fd_in, envp);
		(*tok)->fd_in = 0;
		expand_into_heredoc(tok);
		get_fd_in(tok, envp);
	}
	else
		(*tok)->fd_in = 0;
	if (redir)
		free(redir);
	if (infile)
		free(infile);
}

void	get_fds(t_tokens *lst, t_envp *envp)
{
	t_tokens	*tmp;
	tmp = lst;
	while (tmp)
	{
		get_fd_in(&tmp, envp);
		get_fd_out(&tmp);
		tmp = tmp->next;
	}
}

void	split_into_dlst(t_dlist **lst, char *str, size_t i, size_t j)
{
	t_dlist	*new;
	t_dlist *temp;
	char	*brut;

	new = malloc(sizeof(t_dlist));
	new->next = NULL;
	brut = ft_substr(str, j, i - j);
	new->content = ft_strtrim(brut, " \n\t");
	free(brut);
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

void	remove_fd_lst(t_dlist **lst, char c)
{
	t_dlist *temp;
	t_dlist	*to_delete;
	t_dlist	*to_delete2;

	temp = *lst;
	while (temp->next)
	{
		if (temp->content[0] == c)
		{
			to_delete = temp;
			to_delete2 = temp->next;
			temp = to_delete2->next;
			if (to_delete->prev)
				to_delete->prev->next = to_delete2->next;
			else
			{
				to_delete2->next->prev = NULL;
				*lst = to_delete2->next;
			}
			free(to_delete->content);
            free(to_delete);
            free(to_delete2->content);
            free(to_delete2);
			break ;
		}
		temp = temp->next;
	}
}

char	*join_dlist(t_dlist *lst)
{
	char	*res;
	t_dlist	*temp;

	temp = lst;
	res = ft_strdup(temp->content);
	res = strjoin_free_first(res, " ");
	temp = temp->next;
	while (temp)
	{
		res = strjoin_free_first(res, temp->content);
		if (temp->next)
			res = strjoin_free_first(res, " ");
		temp = temp->next;
	}
	return (res);
}

char	*remove_fd(char *str, char c)
{
	size_t	i;
	size_t 	j;
	t_dlist *lst;
	char	*res;

	i = 0;
	j = 0;
	lst = NULL;
	while (str[i])
	{
		if (is_char_in_set(str[i], "\'\""))
		{
			move_to_next_quote(str, &i, str[i]);
			i++;
			while (is_ws(str[i]))
				i++;
			// ft_printf("IN REMMOVE FD, str[i] = %c, str[i + 1] = %c\n\n", str[i], str[i + 1]);
		}
		else if (is_ws(str[i]))
		{
			split_into_dlst(&lst, str, i, j);
			while (is_ws(str[i]))
				i++;
			j = i;
		}
		else if (str[i] == c)
		{
			split_into_dlst(&lst, str, i, j);
			j = i;
			while (str[i] == c)
				i++;
			//get the redirection
			split_into_dlst(&lst, str, i, j);
			while (is_ws(str[i]))
				i++;
			j = i;
			next_word_end_index(str, &i);
			split_into_dlst(&lst, str, i, j);
			while (is_ws(str[i]))
				i++;
			j = i;
		}
		else
			i++;
	}
	split_into_dlst(&lst, str, i, j);
	remove_fd_lst(&lst, c);
	res = join_dlist(lst);
	free_dlist(&lst);
	return (res);
}

void	remove_redirect(t_tokens *lst)
{
	t_tokens	*temp;
	char		*temp_cnt;

	temp = lst;
	while (temp)
	{
		if (temp->fd_in != STDIN_FILENO)
		{
			temp_cnt = ft_strdup(temp->content);
			free(temp->content);
			temp->content = remove_fd(temp_cnt, '<');
			ft_free_null(temp_cnt);
		}
		if (temp->fd_out != STDOUT_FILENO)
		{
			temp_cnt = ft_strdup(temp->content);
			free(temp->content);
			temp->content = remove_fd(temp_cnt, '>');
			ft_free_null(temp_cnt);
		}
		temp = temp->next;
	}
}

void	split_words_into_lst(t_dlist **lst, char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (is_char_in_set(str[i], "\'\""))
			move_to_next_quote(str, &i, str[i]);
		if (is_ws(str[i]))
		{
			split_into_dlst(lst, str, i, j);
				while (str[i + 1] && is_ws(str[i + 1]))
				i++;
			j = i;
		}
		i++;
	}
	split_into_dlst(lst, str, i, j);
}

void	lst_remove_quotes(t_dlist **lst)
{
	t_dlist *temp;
	char	*res;

	temp = *lst;
	while (temp)
	{
		res = remove_quotes(temp->content);
		ft_free_null(temp->content);
		temp->content = ft_strdup(res);
		ft_free_null(res);
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

void	create_cmd_arr(t_tokens **tk_lst)
{
	t_tokens	*temp;
	char		*to_trim;

	temp = *tk_lst;
		while (temp)
		{
			temp->dlst = NULL;
			to_trim = ft_strtrim(temp->content, " \n\t\b");
			ft_free_null(temp->content);
			temp->content = ft_strdup(to_trim);
			free(to_trim);
			split_words_into_lst(&temp->dlst, temp->content);
			//lst_remove_quotes(&temp->dlst);
			temp->cmd_arr = list_into_arr(temp->dlst);
			free_dlist(&temp->dlst);
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

int	tokenizer(t_mshell *shell)
{
	shell->tok_lst = NULL;
	split_on_pipes(shell, shell->input);
	get_fds(shell->tok_lst, shell->envp);
	remove_redirect(shell->tok_lst);
	print_tkns_down(shell->tok_lst);
	create_cmd_arr(&shell->tok_lst);
	manage_quotes_arr(&shell->tok_lst);
	free_tokens_dlist(&shell->tok_lst);
	give_type(&shell->tok_lst);

	print_single_token(shell->tok_lst);

	return (0);
}

// int tokenizerC(t_mshell *shell)
// {
// 	size_t	i;
// 	size_t	j;
// 	char	c;

// 	i = 0;
// 	j = 0;
// 	shell->tok_lst = NULL;
// 	while (shell->input[i])
// 	{
// 		while (shell->input[i] && !is_sep(shell->input[i]))
// 			i++;
// 		if (create_token(shell, i, j) == 1)
// 			return (1);
// 		if (is_sep(shell->input[i]))
// 		{
// 			if (is_char_in_set(shell->input[i], "<"))
// 			{
// 				j = i;
// 				while (shell->input[i] == '<')
// 					i++;
// 				create_token(shell, i, j);
// 				j = i;
// 				i++;
// 				while (shell->input[i] && !is_ws(shell->input[i]))
// 				{
// 					/*skips chars between quotes*/
// 					if (is_char_in_set(shell->input[i], "\'\""))
// 					{
// 						c = shell->input[i];
// 						while (shell->input[i] && shell->input[i] != c)
// 							i++;
// 						i++;
// 					}
// 					i++;
// 				}
// 				create_token(shell, i, j);
// 				j = i;
// 			}
// 			else
// 			{
// 				j = i;
// 				while (shell->input[i] && is_sep(shell->input[i]))
// 					i++;
// 				if (create_token(shell, i, j) == 1)
// 					return (1);
// 			}
// 		}
// 		else
// 		{
// 			j = i;
// 			while (shell->input[i] && !is_sep(shell->input[i]))
// 			{
// 				if (is_char_in_set(shell->input[i], "\'\""))
// 				{
// 					c = shell->input[i];
// 					while (shell->input[i] && shell->input[i] != c)
// 						i++;
// 					i++;
// 				}
// 			}
// 			if (create_token(shell, i, j) == 1)
// 				return (1);
// 		}
// 		j = i;
// 		if (shell->input[i])
// 			i++;
// 	}
// 	give_type(&shell->tok_lst);
// 	return (0);
// }

// int tokenizerB(t_mshell *shell)
// {
// 	size_t	i;
// 	size_t	j;

// 	i = 0;
// 	j = 0;

// 	shell->tok_lst = NULL;
// 	while (shell->input[i])
// 	{
// 		while (shell->input[i] && !is_sep(shell->input[i]))
// 			i++;
// 		if (create_token(shell, i, j) == 1)
// 			return (1);
// 		if (is_sep(shell->input[i]))
// 		{
// 			if (shell->input[i] == '\'' || shell->input[i] == '\"')
// 			{
// 				j = i;
// 				i = next_quote(shell->input, j, shell->input[i]);
// 				if (shell->input[i] == '\0')
// 					return (ft_printf("Error\nUnclosed quotes"), 1);
// 				i++;
// 				create_token(shell, i, j);
// 			}
// 			else
// 			{
// 				j = i;
// 				while (shell->input[i] && is_sep(shell->input[i]))
// 					i++;
// 				if (create_token(shell, i, j) == 1)
// 					return (1);
// 			}
// 		}
// 		j = i;
// 		if (shell->input[i])
// 			i++;
// 	}
// 	give_type(&shell->tok_lst);
// 	print_tkns_down(shell->tok_lst);
// 	return (0);
// }
