/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:11:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/18 16:59:36 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

char *get_cmd_arguments(char *prompt)
{
	char *result;
	size_t i = 0;
	size_t total_len = ft_strlen(prompt);
	size_t res_len;
	size_t j;

	while (prompt[i] && prompt[i] != ' ')
		i++;
	res_len = total_len - i;
	result = malloc(sizeof(char) * (res_len) + 2);
	j = 0;
	while (prompt[i])
	{
		result[j] = prompt[i];
		i++;
		j++;
	}
	result[j] = '\n';
	result[j + 1] = '\0';
	return (result);
}

int cd_case(t_mshell *shell)
{
	char *str;
	int result;
	char *temp;

	shell->cmd_count++;
	str = get_cmd_arguments(shell->prompt);
	if (!str)
		return (1);
	temp = ft_strtrim(str, "\n");
	if (!temp)
		return (free(str), 1);
	result = chdir(temp);
	if (result != 0)
	{
		ft_printf("minishell: %d: can't cd to %s\n", shell->cmd_count, temp);
		return (free(str), free(temp), 1);
	}

	return (free(str), free(temp), 0);
}