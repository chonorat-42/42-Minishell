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

#include "minishell.h"

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

void	create_envp_list(t_mshell *shell, t_var *var)
{
	t_envp	*new;
	t_envp	*temp;

	new = malloc(sizeof(t_envp));
	if (!new)
		return (free_struct(shell), exit(1));
	if (var->content)
		new->var.content = ft_strdup(var->content);
	new->var.name = ft_strdup(var->name);
	//ft_printf("%s\n", new->var.name);
	new->var.readable = var->readable;
	//ft_printf("%d\n", new->var.readable);
	if (!shell->envp)
	{
		shell->envp = new;
		new->next = NULL;
		new->prev = NULL;
	}
	else
	{
		temp = shell->envp;
		while(temp->next)
			temp = temp->next;
		new->prev = temp;
		temp->next = new;
		new->next = NULL;
	}
}

void	get_envp(t_mshell *shell, char **envp)
{
	size_t	j;
	t_var	new;

	j = 0;
	shell->menvp = NULL;
	shell->envp = NULL;
	shell->cmd = NULL;
	while (envp[j])
	{
		new.name = ft_substr(envp[j], 0, find_char_index(envp[j], '='));
		new.content = ft_substr(envp[j], find_char_index(envp[j], '=') + 1, ft_strlen(envp[j]));
		new.readable = 1;
		create_envp_list(shell, &new);
		free(new.name);
		free(new.content);
		j++;
	}
}
