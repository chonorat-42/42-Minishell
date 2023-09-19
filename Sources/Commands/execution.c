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

#include "minishell.h"

extern int g_status;

char *get_exec(char *cmd)
{
	size_t	j;
	char	*res;

	j = 0;
	while (cmd[j])
		j++;
	while (j != 0 && cmd[j] != '/')
		j--;
	j++;
	res = ft_substr(cmd, j, ft_strlen(cmd));
	return (res);
}

char	*get_path(char *cmd)
{
	size_t	i;
	char	*res;

	i = 0;
	while (cmd[i])
		i++;
	while (cmd[i] != '/')
		i--;
	i++;
	res = ft_substr(cmd, 0, i);
	return (res);
}

void bin_exec(t_mshell *shell, char **cmd_arr, int fd)
{
	size_t	j;
	char	*temp;
	char	*exec;
	char	**exec_split;

	if (fd != 1)
		dup2(fd, STDOUT_FILENO);
	j = -1;
	get_current_location(shell);
	temp = ft_strjoin(shell->current_loc, ft_strtrim(cmd_arr[0], "."));
	if (execve(temp, cmd_arr, NULL) == -1)
		free(temp);
	if (ft_strchr(cmd_arr[0], '/'))
	{
		exec = get_exec(cmd_arr[0]);
		exec_split = ft_split(exec, ' ');
		temp = ft_strjoin(get_path(cmd_arr[0]), exec);
		if (execve(temp, exec_split, NULL) == -1)
		{
			free(exec);
			free_arr(exec_split);
			free(temp);
		}
	}
	if (shell->paths)
	{
		while (shell->paths[++j])
		{
			temp = ft_strjoin(shell->paths[j], cmd_arr[0]);
			if (!temp)
				return (exit(2));
			if (execve(temp, cmd_arr, NULL) == -1)
				free(temp);
		}
	}
	g_status = 127;
	write(shell->p_status[1], &g_status, sizeof(int));
	return (ft_dprintf(STDERR_FILENO, "minishell: %d: %s: command not found\n", shell->cmd_count, cmd_arr[0]), exit(1));
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

void	exec_forwarding(t_tokens *temp, t_mshell *shell, int fd_in, int fd_out)
{
	pid_t	child;
	(void) fd_in;

	if (!ft_strncmp((const char *)temp->content, "echo", 4) && ((!temp->content[4]) || (temp->content[4] && is_ws(temp->content[4]))))
		echo_case(temp->content, fd_out);
	else if (!ft_strncmp(temp->content, "cd", 2) && (is_ws(temp->content[2]) || !temp->content[2]))
		cd_case(shell, temp->content);
	else if (!ft_strcmp(temp->content, "exit"))
		return (ft_putendl_fd("exit", 1), free_struct(shell), exit(0));
	else if (!ft_strcmp(temp->content, "env"))
		env_case(shell, fd_out);
	else if (!ft_strncmp(temp->content, "unset", 5) && is_ws(temp->content[5]))
		unset_case(shell, temp->content);
	else if (!ft_strcmp(temp->content, "pwd"))
		pwd_case(shell, fd_out);
	else if (ft_strncmp(temp->content, "export", 6) == 0)
		export_case(shell, temp->content);
	else
	{
		temp->cmd_arr = ft_split(temp->content, ' ');
		if (!temp->cmd_arr)
			return (free_struct(shell), exit(1));
		pipe(shell->p_status);
		child = fork();
		if (child == -1)
			return(free_struct(shell), exit(2));
		if (!child)
			bin_exec(shell, temp->cmd_arr, fd_out);
		else
			waitpid(child, NULL, 0);
		read(shell->p_status[0], &g_status, sizeof(int));
		close(shell->p_status[0]);
		close(shell->p_status[1]);
		if (temp->cmd_arr)
			free_arr(temp->cmd_arr);
		temp->cmd_arr = NULL;
	}		
}

int	get_final_out(t_tokens *lst)
{
	t_tokens	*temp;
	int			result;

	temp = lst;
	while (temp)
	{
		if (temp->type == RCHEVRON)
		{
			result = open(temp->next->content, O_RDWR | O_CREAT, 0666);
			return (result);
		}
		else if (temp->type == APPEND)
		{
			result = open(temp->next->content, O_RDWR | O_APPEND, 0666);
			return (result);
		}
		temp = temp->next;
	}
	return (1);
}

void execution(t_mshell *shell)
{
	t_tokens	*temp;
	int 		fd_in;
	int			fd_out;

	shell->cmd_count++;
	fd_in = 0;
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
						fd_out = open(temp->next->next->content, O_RDWR | O_CREAT, 0666);
					else
						fd_out = open(temp->next->next->content, O_RDWR | O_APPEND, 0666);
					if (fd_out == -1)
						return (free_struct(shell), exit(4));
				}
			}
			else
				fd_out = 1;
			if (temp->next && temp->next->type == PIPE)
			{
				fd_out = get_final_out(temp);
				handle_pipes(shell, &temp, fd_in, fd_out);
			}
			else
				exec_forwarding(temp, shell, fd_in, fd_out);
		}
		if (fd_out != 1)
		{
			while (temp->next && temp->next->type != CMD)
				temp = temp->next;
		}
		temp = temp->next;
	}
	if (shell->tok_lst)
		ft_free_tokens(&shell->tok_lst);
	if (shell->input)
		free(shell->input);
}
