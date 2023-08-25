/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:53:31 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 17:07:49 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

size_t  get_arr_size(char **arr)
{
	size_t  i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

char    **copy_tab(char **envp, size_t size)
{
	char    **arr;
	size_t  j;
	size_t  k;

	arr = malloc(sizeof(char *) * (size + 1));
	if (!arr)
		return (NULL);
	j = 0;
	while (j < size)
	{
		arr[j] = ft_strdup(envp[j]);
		if (!arr[j])
		{
			k = -1;
			while (++k < j)
				free(arr[k]);
			return (NULL);
		}
		j++;
	}
	arr[size] = NULL;
	return (arr);
}

void    get_envp(t_mshell *shell, char **envp)
{
    if (!envp)
    {
        shell->paths = NULL;
        return ;
    }
	shell->envp_size = get_arr_size(envp);
	shell->menvp = copy_tab(envp, shell->envp_size);
	if (!shell->menvp)
		return (free_struct(shell), exit(2));
}