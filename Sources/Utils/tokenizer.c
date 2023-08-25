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

#include "../../Includes/minishell.h"


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

void give_type(t_tokens **lst)
{
	t_tokens *temp;
	temp = *lst;

	while (temp)
	{
		if (!ft_strcmp(temp->content, "|"))
			temp->type = (int)PIPE;
		else if (!ft_strcmp(temp->content, ">>"))
			temp->type = (int)APPEND;
		else if (!ft_strcmp(temp->content, ">"))
			temp->type = (int)RCHEVRON;
		else if (!ft_strcmp(temp->content, "<<"))
			temp->type = (int)REDIRECT;
		else if (!ft_strcmp(temp->content, "<"))
			temp->type = (int)LCHEVRON;
		temp = temp->next;
	}
	temp = *lst;
	while (lst_type_missing(lst))
	{
		while (temp)
		{
			if (temp->type == 0)
			{
				if (temp->prev && (temp->prev->type == RCHEVRON || temp->prev->type == APPEND))
					temp->type = OUTFILE;
				else if (temp->next && (temp->next->type == LCHEVRON || temp->next->type == REDIRECT))
					temp->type = INFILE;
				else if (temp->content[0] == '\'' || temp->content[0] == '\"')
					temp->type = STRING;
				else if ((temp->next && temp->next->type == PIPE) || (temp->prev && temp->prev->type == PIPE))
					temp->type = CMD;
				else if (temp->prev && (!ft_strncmp(temp->prev->content, "echo", 4) ||
						!ft_strncmp(temp->prev->content, "grep", 4)))
					temp->type = STRING;
				else
					temp->type = CMD;
			}
			temp = temp->next;
		}
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
	while (temp)
	{
		ft_printf("content = %s\n", temp->content);
		ft_printf("type = %d\n", temp->type);
		ft_printf("position = %d\n\n", temp->position);
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
		return (2);
	result = ft_strtrim(str, "\n ");
	free(str);
	if (!result)
		return (1);
	if (str_is_ws(result))
		return (2);
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
		if ("<>|\'\""[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int tokenizer(t_mshell *shell)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (shell->input[i])
	{
		while (shell->input[i] && !is_sep(shell->input[i]))
			i++;
		if (create_token(shell, i, j) == 1)
			return (1);
		if (is_sep(shell->input[i]))
		{
			if (shell->input[i] == '\'' || shell->input[i] == '\"')
			{
				j = i;
				i = next_quote(shell->input, j, shell->input[i]);
				if (shell->input[i] == '\0')
					return (ft_printf("Error\nUnclosed quotes"), 1);
				i++;
				create_token(shell, i, j);
			}
			else
			{
				j = i;
				while (shell->input[i] && is_sep(shell->input[i]))
					i++;
				if (create_token(shell, i, j) == 1)
					return (1);
			}
		}
		j = i;
		if (shell->input[i])
			i++;
	}
	give_type(&shell->tok_lst);
	return (0);
}
