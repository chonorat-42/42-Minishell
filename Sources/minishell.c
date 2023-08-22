/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 10:35:22 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/21 13:31:16 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	g_signum;

size_t get_arr_size(char **arr)
{
	size_t i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

char **copy_tab(char **envp, size_t size)
{
	char **arr;
	size_t j;
	size_t k;

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

void get_envp(t_mshell *shell, char **envp)
{
	shell->envp_size = get_arr_size(envp);
	shell->menvp = copy_tab(envp, shell->envp_size);
	if (!shell->menvp)
		return (free_struct(shell), exit(2));
}


/*mettre get paths en void et delete condition*/
int main(int argc, char **argv, char **envp)
{
	t_mshell shell;
	char *line;

	(void)argc;
	(void)argv;
	sig_handler();
	if (envp)
	{
		get_envp(&shell, envp);
		if (get_paths(&shell, shell.menvp))
			return (free_struct(&shell), 1);
	}
	else
	{
		shell.paths = NULL;
		shell.menvp = NULL;
	}
	while (1)
	{
		ft_printf("minishell:~$ ");
		line = get_next_line(0);
		if (!line)
			return (free_struct(&shell), 1);
		shell.prompt = ft_strtrim((const char *)line, "\n\t\v\f\r ");
		free(line);
		if (!shell.prompt)
			return (free_struct(&shell), 1);
		parsing(&shell);
		tokenizer(&shell);
		execution(&shell, envp);
		free(shell.prompt);
		shell.prompt = NULL;
		//kill(getpid(), SIGQUIT);
	}
	free_struct(&shell);
}