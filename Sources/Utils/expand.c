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

#include "../../Includes/minishell.h"

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

char	*get_other(char *str, size_t *i)
{
	size_t	j;
	char	*temp;

	j = *i + 1;
	while (str[j] && str[j] != '\'' && str[j] != '\"')
		j++;
	temp = ft_substr(str, (*i), j - (*i));
	if (!temp)
		return (NULL);
	*i = j;
	return (temp);
}

char	*expand_envvar(char *str, char **envp)
{

	size_t	i;
	size_t	j;
	char	*bf_envvar;
	char	*env_content;
	char	*res;
	char	*envvar;
	int		envvar_index;
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
		if (ft_strchr(&str[i], '$'))
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
			while (str[i] && !is_ws(str[i]))
				i++;
			envvar = ft_substr(str, j, i - j);
			envvar_index = find_envvar_index(envp, envvar);
			env_content = get_envvar_content(envp[envvar_index], ft_strlen(envvar) + 1);
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
    char	*res;
    char	*temp;
    size_t	i;

    res = NULL;
    i = 0;
    if (!are_all_quotes_closed(shell->input))
		return (ft_printf("Error\nUnclosed quotes\n"), 1);
    while (cmd[i])
    {
        if (cmd[i] == '\'' || cmd[i] == '\"')
            temp = get_quotes(cmd, cmd[i], &i);
        else
        {
            temp = get_other(cmd, &i);
            if (ft_strchr(temp, '$') != NULL)
                temp = expand_envvar(temp, shell->menvp);
        }
        if (!res)
            res = ft_strdup(temp);
        else
            res = ft_strjoin(res, temp);
        free(temp);
    }
    free(shell->input);
    shell->input = ft_strdup(res);
	free(res);
	return (0);
}