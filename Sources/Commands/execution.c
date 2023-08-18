/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:48:14 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/18 13:18:12 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

char *get_cmd_arguments(char *prompt)
{
	char *result;
	size_t i = 0;
	size_t total_len = ft_strlen(prompt);
	size_t res_len;
	size_t j;

	while (prompt[i] && prompt[i] != ' ')
		i++;
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

int bin_exec(t_mshell shell, char **cmd_arr, char **envp, int fd)
{
	size_t j;
	char *temp;

	if (fd != 1)
		dup2(fd, STDOUT_FILENO);
	j = -1;
	while (shell.paths[++j])
	{
		temp = ft_strjoin(shell.paths[j], cmd_arr[0]);
		if (!temp)
			return (2);
		execve(temp, cmd_arr, envp);
		free(temp);
	}
	close(fd);
	return (ft_printf("sh: %d: %s: not found\n", shell.cmd_count, cmd_arr[0]), 1);
}

char *get_cmd(char *str, size_t *i)
{
	char *result;
	size_t res_len;
	size_t j;

	while (str[(*i)] != ' ')
		(*i)++;
	if (str[(*i) + 1] == '-')
	{
		while (str[(*i)] != ' ')
			(*i)++;
	}
	res_len = (*i) + 1;
	result = malloc(sizeof(char) * res_len);
	if (!result)
		return (NULL);
	result[res_len - 1] = '\0';
	j = 0;
	while (j < res_len)
	{
		result[j] = str[j];
		j++;
	}
	return (result);
}

/*to do :
- ENVP dans structure
- lister les builtins
- echo a refaire */
void execution(t_mshell *shell, char **envp)
{
	t_tokens *temp;
	pid_t child;
	int fd;

	temp = shell->tok_lst;
	while (temp)
	{
		if (temp->type == CMD)
		{
			if (temp->next && (temp->next->type == APPEND || temp->next->type == RCHEVRON))
			{
				if (temp->next->next)
				{
					if (temp->next->type == RCHEVRON)
						fd = open(temp->next->next->content, O_RDWR | O_CREAT, 0666);
					else
						fd = open(temp->next->next->content, O_RDWR | O_APPEND, 0666);
					if (fd == -1)
						return (free_struct(shell), exit(4));
				}
			}
			else
				fd = 1;
			shell->cmd_count++;
			if (!ft_strncmp((const char *)temp->content, "echo", 4))
			{
				echo_case(temp->content, fd);
				if (fd != 1)
				{
					while (temp && temp->type != CMD)
						temp = temp->next;
				}
			}
			else if (!ft_strncmp((const char *)temp->content, "cd", 2))
				cd_case(shell);
			else if (!ft_strncmp((const char *)temp->content, "exit", 4))
				return (free_struct(shell), exit(0));
			else if (!ft_strncmp((const char *)temp->content, "env", 3))
				env_case(shell);
			else
			{
				temp->cmd_arr = ft_split(temp->content, ' ');
				if (!temp->cmd_arr)
					return (free_struct(shell), exit(1));
				child = fork();
				if (child == -1)
					return (free_struct(shell), exit(2));
				if (child == 0)
				{
					if (bin_exec(*shell, temp->cmd_arr, envp, fd))
						return (free_struct(shell), exit(4));
				}
				else
					waitpid(child, NULL, 0);
				if (temp->cmd_arr)
					free_arr(temp->cmd_arr);
				temp->cmd_arr = NULL;
			}
		}
		temp = temp->next;
	}
	ft_free_tokens(&shell->tok_lst);
}
