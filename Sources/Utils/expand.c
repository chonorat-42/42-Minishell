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
	while (str[i] != '$')
		i++;
	if (ft_isalnum(str[i + 1]))
		return (1);
	return (0);
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
		if (ft_strchr(&str[i], '$') && check_after_dollar(&str[i]))
		{

			/*s'il y a un $ dans la suite de la string
			j = debut du reste de la string*/

			j = i;
			i = find_char_index(&str[i], '$');
			i += j;

			/*i = index de $*/

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

			/*j et i = premier caractere de la variable 'environnement*/


			while (str[i] && !is_ws(str[i]) && str[i] != '\'' && str[i] != '\"')
				i++;
			envvar = ft_substr(str, j, i - j);

			// ft_printf("ENVVAR = %s\n\n", envvar);

			if (envvar && envvar[0])
			{
				envvar_index = find_envvar_index(envp, envvar);
				env_content = get_envvar_content(envp[envvar_index], ft_strlen(envvar) + 1);
			}
			else
				env_content = ft_strdup("");

			// ft_printf("env content = %s\n\n", env_content);
			
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
    size_t	i;

	temp = NULL;
    i = 0;
    if (!are_all_quotes_closed(cmd))
		return (ft_printf("Error\nUnclosed quotes\n"), 1);
    if (ft_strchr(cmd, '$') != NULL)
	{
		if (check_after_dollar(cmd))
		{
    		temp = expand_envvar(cmd, shell->menvp);
    		free(shell->input);
    		shell->input = ft_strdup(temp);
			free(temp);
		}
	}
	return (0);
}