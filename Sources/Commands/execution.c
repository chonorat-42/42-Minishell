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

// void	exec_forwardingB(t_tokens *temp, t_mshell *shell, int fd_in, int fd_out)
// {
// 	pid_t	child;

// 	// ft_printf("in exec forwarding, temp->content = %s\n\n", temp->content);

// 	if (!ft_strncmp((const char *)temp->content, "echo", 4) && ((!temp->content[4]) || (temp->content[4] && is_ws(temp->content[4]))))
// 		echo_case(temp->content, fd_out);
// 	else if (!ft_strncmp(temp->content, "cd", 2) && (is_ws(temp->content[2]) || !temp->content[2]))
// 		cd_case(shell, temp->content);
// 	else if (!ft_strncmp(temp->content, "exit", 4))
// 		exit_case(shell, temp->content);
// 	else if (!ft_strcmp(temp->content, "env"))
// 		env_case(shell, fd_out);
// 	else if (!ft_strncmp(temp->content, "unset", 5) && is_ws(temp->content[5]))
// 		unset_case(shell, temp->content);
// 	else if (!ft_strcmp(temp->content, "pwd"))
// 		pwd_case(shell, fd_out);
// 	else if (ft_strncmp(temp->content, "export", 6) == 0)
// 		export_case(shell, temp->content);
// 	else
// 	{
// 		temp->cmd_arr = ft_split(temp->content, ' ');
// 		if (!temp->cmd_arr)
// 			return (free_struct(shell), exit(1));
// 		child = fork();
// 		if (child == -1)
// 			return(free_struct(shell), exit(2));
// 		if (!child)
// 			bin_exec(shell, temp->cmd_arr, fd_in, fd_out);
// 		else
// 			waitpid(child, &g_status, 0);
// 		if (WIFEXITED(g_status))
// 			g_status = WEXITSTATUS(g_status);
// 		else if (WIFSIGNALED(g_status))
// 			g_status = WTERMSIG(g_status);
// 		if (temp->cmd_arr)
// 			free_arr(temp->cmd_arr);
// 		temp->cmd_arr = NULL;
// 	}		
// }

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
			handle_pipes(shell, &temp, temp->fd_in, temp->fd_out);
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

// void executionB(t_mshell *shell)
// {
// 	t_tokens	*temp;
// 	int 		fd_in;
// 	int			fd_out;

// 	shell->cmd_count++;
// 	fd_in = STDIN_FILENO;
// 	fd_out = STDOUT_FILENO;
// 	temp = shell->tok_lst;
// 	while (temp)
// 	{
// 		if (temp->type == CMD)
// 		{
// 			if (temp->next && (temp->next->type == APPEND || temp->next->type == RCHEVRON))
// 			{
// 				if (temp->next->next)
// 				{
// 					if (temp->next->type == RCHEVRON)
// 						fd_out = open(temp->next->next->content, O_RDWR | O_CREAT, 0666);
// 					else
// 						fd_out = open(temp->next->next->content, O_RDWR | O_APPEND, 0666);
// 					if (fd_out == -1)
// 						return (free_struct(shell), exit(4));
// 				}
// 			}
// 			else if (temp->next && (temp->next->type == LCHEVRON))
// 			{
// 				if (temp->next->next)
// 				{
// 					fd_in = open(temp->next->next->content, O_RDONLY);
// 				}
// 				if (fd_in == -1)
// 						return (free_struct(shell), exit(4));
// 			}
// 			else if (temp->next && (temp->next->type == HEREDOC))
// 			{
// 				if (temp->next->next)
// 				{
// 					fd_in = open("/tmp/temp.heredoc", O_RDWR | O_CREAT, 0666);
// 					heredoc(temp->next->next->content, fd_in, shell->envp);
// 					temp->content = ft_strjoin(strjoin_free_first(temp->content, " "), "/tmp/temp.heredoc");
// 					remove_heredoc_tkn(&temp);
// 					if (temp->next && temp->next->type == PIPE)
// 					{
// 						handle_pipes(shell, &temp, 0, fd_out);
// 						temp->content = ft_strdup("rm -rf /tmp/temp.heredoc");
// 						temp->next = NULL;
// 						exec_forwarding(temp, shell, fd_in, fd_out);
// 						free(temp->content);
// 						temp = temp->next;
// 					}
// 					else
// 					{
// 						exec_forwarding(temp, shell, fd_in, fd_out);
// 						free(temp->content);
// 						temp->content = ft_strdup("rm -rf /tmp/temp.heredoc");
// 						exec_forwarding(temp, shell, fd_in, fd_out);
// 						free(temp->content);
// 						close(fd_in);
// 					}
// 					close(fd_in);
// 				}
// 			}
// 			else
// 			{
// 				fd_out = 1;
// 				fd_in = 0;
// 			}
// 			// if (fd_out != STDOUT_FILENO)
// 			// {
// 			// 	while (temp->next && temp->next->type != CMD)
// 			// 		temp = temp->next;
// 			// }
// 			if (fd_in != STDIN_FILENO)
// 			{
// 				while (temp && temp->next && (temp->next->type != CMD && temp->next->type != PIPE))
// 					temp = temp->next;
// 			}
// 			if (temp && temp->next && temp->next->type == PIPE)
// 			{
// 				fd_out = get_final_out(temp);
// 				handle_pipes(shell, &temp, fd_in, fd_out);
// 			}
// 			else
// 			{
// 				if (temp && temp->type != CMD)
// 					temp = temp->next;
// 				if (temp)
// 				{
// 					exec_forwarding(temp, shell, fd_in, fd_out);
// 				}
// 			}
// 		}
// 		if (temp)
// 			temp = temp->next;
// 	}
// 	if (shell->tok_lst)
// 		ft_free_tokens(&shell->tok_lst);
// 	if (shell->input)
// 		free(shell->input);
// 	// dup2(STDIN_FILENO, 0);
// 	// dup2(STDOUT_FILENO, 1);
// }
