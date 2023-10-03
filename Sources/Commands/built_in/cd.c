/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:11:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/20 19:29:00 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

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
	while (prompt[i])
	{
		result[j] = prompt[i];
		i++;
		j++;
	}
	result[j] = '\0';
	return (result);
}

void cd_case(t_mshell *shell, char **cmd)
{
	char	*home;

	if (count_arr_size(cmd) > 1)
		if (chdir(cmd[1]))
			builtin_error(cmd[0], cmd[1], 1);
	else
	{
		home = get_envvar_content(shell->envp, "HOME");
		if (!home[0])
			return (free(home), builtin_error(cmd[0], cmd[1], 2));
		
	}
}