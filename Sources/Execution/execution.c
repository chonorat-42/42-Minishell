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

extern long long g_status;

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

void bin_exec(t_mshell *shell, char **cmd_arr, int fd_in, int fd_out)
{
	size_t	j;
	char	*temp;
	char	*trim;
	char	*exec;
	char	**exec_split;

	if (fd_out != 1)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (fd_in != 0)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	j = -1;
	if (!get_current_location(shell))
		return (free_struct(shell), exit(1));
	trim = ft_strtrim(cmd_arr[0], ".");
	temp = ft_strjoin(shell->current_loc, trim);
	free(trim);
	if (execve(temp, cmd_arr, shell->menvp) == -1)
		free(temp);
	if (ft_strchr(cmd_arr[0], '/'))
	{
		exec = get_exec(cmd_arr[0]);
		exec_split = ft_split(exec, ' ');
		temp = ft_strjoin(get_path(cmd_arr[0]), exec);
		if (execve(temp, exec_split, shell->menvp) == -1)
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
				return (free_struct(shell), exit(1));
			if (execve(temp, cmd_arr, shell->menvp) == -1)
				free(temp);
		}
		show_error(cmd_arr[0], "EXEC", 1);
	}
	else
		show_error(cmd_arr[0], "EXEC", 2);
	free_struct(shell);
	if (fd_out != 1)
		close(fd_out);
	if (fd_in != 0)
		close(fd_in);
	exit(127);
}

void	print_single_token(t_tokens *tkn)
{
	if (DEBUG)
	{
		ft_printf("content = %s\n", tkn->content);
		ft_printf("cmd_arr :\n_________________\n");
		print_arr(tkn->cmd_arr);
		ft_printf("_________________\n");
		ft_printf("fd_in = %d, fd_out = %d\n", tkn->fd_in, tkn->fd_out);
	}
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

void	exec_forwarding(t_tokens *temp, t_mshell *shell)
{
	pid_t	child;

	if (!ft_strcmp(temp->cmd_arr[0], "echo"))
		echo_case(temp->cmd_arr, temp->fd_out);
	else if (!ft_strcmp(temp->cmd_arr[0], "cd"))
		cd_case(shell, temp->cmd_arr);
	else if (!ft_strcmp(temp->cmd_arr[0], "exit"))
		exit_case(shell, temp->cmd_arr);
	else if (!ft_strcmp(temp->cmd_arr[0], "env"))
		env_case(shell, temp->cmd_arr, temp->fd_out);
	else if (!ft_strcmp(temp->cmd_arr[0], "unset"))
		unset_case(shell, temp->cmd_arr);
	else if (!ft_strcmp(temp->cmd_arr[0], "pwd"))
		pwd_case(shell, temp->cmd_arr, temp->fd_out);
	else if (!ft_strcmp(temp->cmd_arr[0], "export"))
		export_case(shell, temp->cmd_arr, temp->fd_out);
	else
	{
		child = fork();
		if (child == -1)
			return (free_struct(shell), exit(2));
		if (!child)
			bin_exec(shell, temp->cmd_arr, temp->fd_in, temp->fd_out);
		else
			waitpid(child, (int *)&g_status, 0);
		if (WIFEXITED(g_status))
			g_status = WEXITSTATUS(g_status);
		else if (WIFSIGNALED(g_status))
			g_status = WTERMSIG(g_status);
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

void	free_single_token(t_tokens *node)
{
	free(node->content);
	free(node);
}

void	remove_next_token(t_tokens **lst)
{
	t_tokens *next;

	if ((*lst)->next)
		next = (*lst)->next->next;
	else
		next = NULL;
	if ((*lst)->next && (*lst)->next->next)
		(*lst)->next->next->prev = *lst;
	free_single_token((*lst)->next);
	(*lst)->next = next;
}

void	remove_heredoc_tkn(t_tokens **lst)
{
	while ((*lst)->next && ((*lst)->next->type == HEREDOC || (*lst)->next->type == HEREDEL))
		remove_next_token(lst);
}

void	execution(t_mshell *shell)
{
	t_tokens	*temp;

	exec_sig();
	temp = shell->tok_lst;
	while (temp)
	{
		if (temp->next && temp->next->type == PIPE)
		{
			// print_single_token(temp);
			handle_pipes(shell, &temp, &temp->fd_in, &temp->fd_out);
		}
		else
			exec_forwarding(temp, shell);
		temp = temp->next;
	}
	free(shell->input);
	ft_free_tokens(&shell->tok_lst);
	if (shell->paths)
	{
		free_arr(shell->paths);
		shell->paths = NULL;
	}
}
