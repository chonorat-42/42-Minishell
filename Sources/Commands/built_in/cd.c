/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 13:11:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/20 14:51:44 by pgouasmi         ###   ########.fr       */
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
	temp = ft_strtrim(str, "\n\t\v\f\r ");
	free(str);
	if (!temp)
		return (1);
	ft_printf("in cd case, temp = %s\n", temp);
	result = chdir(temp);
	if (result != 0)
	{
		ft_printf("minishell: %d: can't cd to %s\n", shell->cmd_count, temp);
		free(temp);
		return (1);
	}
	free(temp);
	return (0);
}