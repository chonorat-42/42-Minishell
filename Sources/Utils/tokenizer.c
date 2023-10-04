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

void	heredoc_into_infile(t_dlist **lst)
{
	t_dlist	*temp;

	temp = *lst;
	while (ft_strcmp(temp->content, "<<"))
		temp = temp->next;
	free(temp->content);
	temp->content = ft_strdup("<");
	temp = temp->next;
	free(temp->content);
	temp->content = ft_strdup("/tmp/temp.heredoc");
}

void	get_fd_in(t_tokens **tok, t_envp *envp)
{
	t_tokens	*temp_tok;
	t_dlist		*temp_dlst;
	int			temp_fd;
	int			has_fd;
	
	temp_tok = *tok;
	while (temp_tok)
	{
		temp_dlst = temp_tok->dlst;
		has_fd = 0;
		temp_fd= 0;

		while (temp_dlst)
		{
			if (temp_dlst->content[0] == '<' && ft_strlen(temp_dlst->content) == 1)
			{
				if (has_fd)
					close(temp_fd);
				temp_fd = open(temp_dlst->next->content, O_RDWR);
				has_fd++;
			}	
			else if (temp_dlst->content[0] == '<' && ft_strlen(temp_dlst->content) == 2)
			{
				temp_fd = open("/tmp/temp.heredoc", O_RDWR | O_CREAT, 0666);
				heredoc(temp_dlst->next->content, temp_fd, envp);
				heredoc_into_infile(&(*tok)->dlst);
				get_fd_in(tok, envp);
			}
			temp_dlst = temp_dlst->next;
		}
		temp_tok->fd_in = temp_fd;
		temp_tok = temp_tok->next;
	}
}

void	get_fd_out(t_tokens **tok)
{
	t_tokens	*temp_tok;
	t_dlist		*temp_dlst;
	int			temp_fd;
	int			has_fd;
	
	temp_tok = *tok;
	while (temp_tok)
	{
		temp_dlst = temp_tok->dlst;
		has_fd = 0;
		temp_fd= 1;
		while (temp_dlst)
		{
			if (temp_dlst->content[0] == '>' && ft_strlen(temp_dlst->content) == 1)
			{
				if (has_fd)
					close(temp_fd);
				temp_fd = open(temp_dlst->next->content, O_RDWR | O_CREAT | O_TRUNC, 0666);
				has_fd++;
			}
			else if (temp_dlst->content[0] == '>' && ft_strlen(temp_dlst->content) == 2)
			{
				if (has_fd)
					close(temp_fd);
				temp_fd = open(temp_dlst->next->content, O_RDWR | O_APPEND);
				has_fd++;
			}
			temp_dlst = temp_dlst->next;
		}
		temp_tok->fd_out = temp_fd;
		temp_tok = temp_tok->next;
	}
}

void	get_fds(t_tokens **lst, t_envp *envp)
{
		get_fd_in(lst, envp);
		get_fd_out(lst);
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

// void	free_next_two_nodes(t_dlist	**to_delete,t_dlist	**to_delete2)
// {
// 	free((*to_delete)->content);
//     free(*to_delete);
//     free((*to_delete2)->content);
//     free(*to_delete2);
// }

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

void	create_cmd_arr(t_tokens **tk_lst)
{
	t_tokens	*temp;

		temp = *tk_lst;
		while (temp)
		{
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

void	split_words_and_redir(t_dlist **lst, char *str)
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

void	split_input_into_dlst(t_tokens **lst)
{
	t_tokens	*temp;

	temp = *lst;
	while (temp)
	{
		temp->dlst = NULL;
		split_words_and_redir(&temp->dlst, temp->content);
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

/*to fix : several redirect --> keep the last one
leak redaline de heredoc */

int	tokenizer(t_mshell *shell)
{
	shell->tok_lst = NULL;
	split_on_pipes(shell, shell->input);
	split_input_into_dlst(&shell->tok_lst);
	get_fds(&shell->tok_lst, shell->envp);
	remove_redirect(&shell->tok_lst);
	// ft_printf("after remove redirect :\n");
	// print_tokens_dlist(shell->tok_lst);
	create_cmd_arr(&shell->tok_lst);
	manage_quotes_arr(&shell->tok_lst);
	free_tokens_dlist(&shell->tok_lst);
	give_type(&shell->tok_lst);
	return (0);
}
