/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:26:35 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 18:06:50 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_quotes(char *str, int c, size_t *i)
{
	char	*result;
	int		j;

	j = *i;
    (*i)++;
	if (str[(*i)] == c)
	{
		result = ft_strdup("");
		return (result);
	}
	while (str[(*i)] && str[(*i)] != c)
		(*i)++;
    (*i)++;
	result = ft_substr(str, j, *i - j);
	if (!result)
		return (NULL);
	return (result);
}

int	are_all_quotes_closed(char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"'))
		{
			j = i + 1;
			while (str[j] && str[j] != str[i])
				j++;
			if (!str[j])
				return (0);
			i = j;
		}
		i++;
	}
	return (1);
}

int	check_after_dollar(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	if (ft_isalnum(str[i + 1]))
		return (1);
	return (0);
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

	i = 0;
	while (str[i])
	{
		if (is_ws(str[i]) || is_char_in_set(str[i], "\"\'="))
		{
			return (i);
		}
		i++;
	}
	return (i);
}

// void	add_to_list(t_list **lst, char *str)
// {
// 	t_list	*new;
// 	t_list	*temp;

// 	new = malloc(sizeof(t_list));
// 	if (!(*lst))
// 		*lst = new;
// 	else
// 	{
// 		temp = *lst;
// 		while (temp)
// 			temp = temp->next;
// 	}
// 	(char *)new->content = ft_strdup(str);
// 	temp->next = NULL;
// 	free(str); 
// }

/*creer liste chainee
  join les differents nodes*/
// char	*expand_envvar(char *str, t_envp *envp)
// {
// 	size_t	i;
// 	size_t	j;
// 	t_list	*lst;
// 	char	*res;
// 	char	*envvar;

// 	res = NULL;
// 	i = 0;
// 	while (str[i])
// 	{
// 		j = i;
// 		if (find_char_index(&str[i], '$') >= 0)
// 		{
// 			i += find_char_index(&str[i], '$');
// 			add_to_lst(&lst, ft_substr(str, j, i - j));
// 			i++;
// 			j = 0;
// 			i += last_envvar_char(&str[i]);
// 			temp = ft_substr(str, j, i - j);
// 			if (!temp)
// 				temp = ft_strdup("");
			
// 		}
// 	}
// }

char	*expand_envvar(char *str, t_envp *envp)
{
	size_t	i;
	size_t	j;
	char	*temp;
	char	*join;
	char	*res;

	res = NULL;
	i = 0;
	while (str[i])
	{
		j = i;
		if (find_char_index(&str[i], '$') >= 0)
		{
			i += find_char_index(&str[i], '$');
			temp = ft_substr(str, j, i - j);
			join = ft_strdup(temp);
			free(temp);
			temp = NULL;
			i++;
			j = i;
			i += last_envvar_char(&str[i]);
			temp = ft_substr(str, j, i - j);
			if (!temp)
				temp = ft_strdup("");
			join = strjoin_free_both(join, get_envvar_content(envp, temp));
			if (res)
				res = strjoin_free_first(res, join);
			else
				res = ft_strdup(join);
			if (join)
			{
				free(join);
				join = NULL;
			}
			if (temp)
			{
				free(temp);
				temp = NULL;
			}
		}
		else
		{
			j = i;
			while (str[i])
				i++;
			temp = ft_substr(str, j, i - j);
			if (res)
				res = strjoin_free_first(res, temp);
			else
				res = ft_strdup(temp);
			// if (temp)
			// 	ft_free_null(temp);
		}
		if (join)
		{
			free(join);
			join = NULL;
		}
		if (temp)
		{
			free(temp);
			temp = NULL;
		}
	}
	if (join)
	{
		free(join);
		join = NULL;
	}
	if (temp)
	{
		free(temp);
		temp = NULL;
	}
	return (res);
}

size_t	count_char(char *str, char c)
{
	size_t	i;
	size_t	res;
	char	q;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (is_char_in_set(str[i], "\'\""))
		{
			q = str[i];
			i++;
			while (str[i] && str[i] != q)
				i++;
		}
		else if (str[i] == c)
			res++;
		i++;
	}
	return (res);
}

char	*remove_char(char *str, char c)
{
	size_t	i;
	size_t	j;
	size_t	to_delete;
	char	*res;
	char	q;

	to_delete = count_char(str, c);
	i = 0;
	j = 0;
	res = malloc(sizeof(char) * (ft_strlen(str) - to_delete));
	while(str[i])
	{
		if (str[i] == c)
			i++;
		else if (is_char_in_set(str[i], "\'\""))
		{
			res[j] = str[i];
			q = str[i];
			while (str[i] && str[i] != q)
			{
				res[j] = str[i];
				i++;
				j++;
			}
			res[j] = str[i];
			i++;
			j++;
		}
		else
		{
			res[j] = str[i];
			j++;
			i++;
		}
	}
	res[j] = '\0';
	return (res);
}

int	expand(t_mshell *shell, char *cmd)
{
    char	*temp;

	temp = NULL;
    if (!are_all_quotes_closed(cmd))
		return (ft_printf("Error\nUnclosed quotes\n"), 1);
    if (find_char_index(cmd, '$') >= 0)
	{
		if (check_after_dollar(cmd))
		{
    		temp = expand_envvar(cmd, shell->envp);
    		free(shell->input);
    		shell->input = ft_strdup(temp);
			free(temp);
		}
		else
		{
			temp = remove_char(cmd, '$');
			free(cmd);
			cmd = ft_strdup(temp);
			free(temp);
		}
	}
	return (0);
}