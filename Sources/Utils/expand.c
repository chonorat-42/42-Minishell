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
			return (i);
		i++;
	}
	return (i);
}

char	*strjoin_free(char *tf_s1, char *s2)
{
	size_t	i;
	size_t	j;
	size_t	size;
	char	*res;

	size = ft_strlen(tf_s1) + ft_strlen(s2);
	res = malloc(sizeof(char) * (size + 1));
	i = 0;
	j = 0;
	while (tf_s1[i])
	{
		res[j] = tf_s1[i];
		i++;
		j++;
	}
	free(tf_s1);
	i = 0;
	while (s2[i])
	{
		res[j] = s2[i];
		i++;
		j++;
	}
	res[j] = '\0';
	return (res);
}

char	*expand_envvar2(char *str, t_envp *envp)
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
			i = (size_t)find_char_index(&str[i], '$');
			temp = ft_substr(str, j, i - j);
			join = ft_strdup(temp);
			free(temp);
			i++;
			j = i;
			i += last_envvar_char(&str[i]);
			temp = ft_substr(str, j, last_envvar_char(&str[i]) - j);
			join = strjoin_free(join, get_envvar_content(envp, temp));
			if (res)
				res = strjoin_free(res, join);
			else
				res = ft_strdup(join);
			free(join);
			free(temp);
			// if (str[i])
			// 	i++;
		}
		else
		{
			j = i;
			while (str[i])
				i++;
			temp = ft_substr(str, j, i - j);
			if (res)
				res = strjoin_free(res, temp);
			else
				res = ft_strdup(temp);
		}
		if (join)
			free(join);		
	}
	return (res);
}

char	*expand_envvar(char *str, t_envp *envp)
{

	size_t	i;
	size_t	j;
	char	*bf_envvar;
	char	*env_content;
	char	*res;
	char	*envvar;
	char	*temp;

	bf_envvar = NULL;
	env_content = NULL;
	res = NULL;
	envvar = NULL;
	temp = NULL;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (ft_strchr(&str[i], '$') && check_after_dollar(&str[i]))
		{
			j = i;
			i = find_char_index(&str[i], '$');
			i += j;
			bf_envvar = ft_substr(str, j, i - j);
			if (res && bf_envvar)
				res = ft_strjoin(res, bf_envvar);
			else if (!res && bf_envvar)
				res = ft_strdup(bf_envvar);
			else if (!res && !bf_envvar)
				res = NULL;
			if (bf_envvar)
				free(bf_envvar);
			i++;
			j = i;
			while (str[i] && !is_ws(str[i]) && str[i] != '\'' && str[i] != '\"')
				i++;
			envvar = ft_substr(str, j, i - j);
			if (envvar && envvar[0])
				env_content = get_envvar_content(envp, envvar);
			else
				env_content = ft_strdup("");
			if (envvar)
				free(envvar);
			j = i;
			if (res && env_content)
				res = ft_strjoin(res, env_content);
			else if (!res && env_content)
				res = ft_strdup(env_content);
			else if (bf_envvar && !env_content)
				res = ft_strdup(bf_envvar);
			else
				res = NULL;
			if (env_content)
				free(env_content);
		}
		else
		{
			while (str[i])
				i++;
			temp = ft_substr(str, j, i - j);
			if (res && temp)
				res = ft_strjoin(res, temp);
			if (!res && temp)
				res = ft_strdup(temp);
			if (!res && !temp)
				res = NULL;
			if (temp)
				free(temp);
		}
	}
	return (res);
}

int	expand(t_mshell *shell, char *cmd)
{
    char	*temp;

	temp = NULL;
    if (!are_all_quotes_closed(cmd))
		return (ft_printf("Error\nUnclosed quotes\n"), 1);
    if (find_char_index(cmd, '$'))
	{
		if (check_after_dollar(cmd))
		{
    		temp = expand_envvar2(cmd, shell->envp);
    		free(shell->input);
    		shell->input = ft_strdup(temp);
			free(temp);
		}
	}
	return (0);
}