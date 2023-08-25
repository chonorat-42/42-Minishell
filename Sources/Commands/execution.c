/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:48:14 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 19:06:58 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void bin_exec(t_mshell shell, char **cmd_arr, char **envp, int fd)
{
	size_t j;
	char *temp;

	if (fd != 1)
		dup2(fd, STDOUT_FILENO);
	j = -1;
	if (shell.paths)
	{
		while (shell.paths[++j])
		{
			temp = ft_strjoin(shell.paths[j], cmd_arr[0]);
			if (!temp)
				return (exit(2));
			if (execve(temp, cmd_arr, envp) == -1)
				free(temp);
		}
	}
	return (ft_dprintf(STDERR_FILENO, "minishell: %d: %s: command not found\n", shell.cmd_count, cmd_arr[0]), exit(1));
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
- debugger bin_exec double execution DONE
- builtin sans arg KO DONE
- refaire expand DONE
- gerer simple quotes echo
- export (diff export env ?)
- gestion des pipes
- factorisation + cleaning*/
void execution(t_mshell *shell)
{
	t_tokens	*temp;
	int			fd;
	pid_t		child;

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
			if (!ft_strncmp((const char *)temp->content, "echo", 4) && ((!temp->content[4]) || (temp->content[4] && is_ws(temp->content[4]))))
			{
				if (echo_case(temp->content, fd))
					return (free_struct(shell), exit(6));
				if (fd != 1)
				{
					while (temp && temp->type != CMD)
						temp = temp->next;
				}
			}
			else if (!ft_strncmp(temp->content, "cd", 2) && (is_ws(temp->content[2]) || !temp->content[2]))
				cd_case(shell, temp->content);
			else if (!ft_strcmp(temp->content, "exit"))
				return (free_struct(shell), exit(0));
			else if (!ft_strcmp(temp->content, "env"))
				env_case(shell, temp->content);
			else if (!ft_strncmp(temp->content, "unset", 5) && is_ws(temp->content[5]))
				unset_case(shell, temp->content);
			else if (!ft_strcmp(temp->content, "pwd"))
				pwd_case(shell);
			else
			{
				temp->cmd_arr = ft_split(temp->content, ' ');
				if (!temp->cmd_arr)
					return (free_struct(shell), exit(1));
				child = fork();
				if (child == -1)
					return(free_struct(shell), exit(2));
				if (!child)
					bin_exec(*shell, temp->cmd_arr, shell->menvp, fd);
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
	free(shell->prompt);
}
