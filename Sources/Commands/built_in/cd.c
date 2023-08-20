/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:11:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/20 15:28:21 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

char *get_home(char **envp)
{
	int		index;
	char	*res;

	index = find_envvar_index(envp, "HOME");
	if (index < 0)
	{
		res = malloc(sizeof(char) * 1);
		if (!res)
			return (NULL);
		res[0] = '\0';
		return (res);
	}
	res = get_envvar_content(envp[index], 5);
	if (!res)
		return (NULL);
	return (res);

}

char *get_cmd_arguments(char *prompt)
{
	char *result;
	size_t i = 0;
	size_t total_len = ft_strlen(prompt);
	size_t res_len;
	size_t j;

	while (prompt[i] && prompt[i] != ' ')
		i++;
	j = total_len;
	while (is_ws(prompt[j]))
		j--;
	res_len = j - i;
	result = malloc(sizeof(char) * (res_len) + 1);
	j = 0;
	while (prompt[i] )
	{
		result[j] = prompt[i];
		i++;
		j++;
	}
	result[j] = '\0';
	return (result);
}

int cd_case(t_mshell *shell)
{
	char *temp;
	char *str;
	int result;

	shell->cmd_count++;
	if (ft_strlen(shell->prompt) == 2)
		str = get_home(shell->menvp);
		if (str && !str[0])
			return (ft_printf("Error/nPath to HOME could not be found\n"), 2);
	else
		temp = get_cmd_arguments(shell->prompt);
	if (!temp)
		return (3);
	str = ft_strtrim(temp, "\n\t\v\f\r ");
	free(temp);
	if (!str)
		return (3);
	result = chdir(str);
	if (result != 0)
	{
		ft_printf("minishell: %d: can't cd to %s\n", shell->cmd_count, str);
		free(str);
		return (3);
	}
	free(str);
	return (0);
}