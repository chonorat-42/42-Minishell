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
		if (is_ws(str[i]) || is_char_in_set(str[i], "\"\'"))
		{
			return (i);
		}
		i++;
	}
	return (i);
}

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
		if (find_char_index(&str[i], '$' >= 0))
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
		}
		if (join)
		{
			free(join);
			join = NULL;
		}
		if (temp)
		{
			free(temp);
			res = NULL;
		}
	}
	if (join)
	{
		free(join);
		join = NULL;
	}
	if (temp)
	{
		free(res);
		res = NULL;
	}
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
	}
	return (0);
}