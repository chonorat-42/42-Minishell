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

void cd_case(t_mshell *shell, char **cmd)
{
	char	*home;
	int		arg_count;

	arg_count = count_arr_size(cmd);
	if (arg_count > 1)
	{
		if (arg_count > 2)
			return (builtin_error(cmd[0], cmd[1], 3));
		if (chdir(cmd[1]))
			builtin_error(cmd[0], cmd[1], 1);
	}
	else
	{
		home = get_envvar_content(shell->envp, "HOME");
		if (!home[0])
			return (free(home), builtin_error(cmd[0], cmd[1], 2));
		if (chdir(home))
			builtin_error(cmd[0], home, 1);
		free(home);
	}
}
