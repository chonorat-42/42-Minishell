/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:48:14 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/15 16:09:40 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int are_quotes_closed(char *str)
{
	size_t len = ft_strlen(str);
	size_t i = 0;
	size_t j = len;

	while (str[i] && i != j)
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			while (str[j] != '\'' && str[j] != '\"' && j > 0)
				j--;
			if (str[i] != str[j] || j == i)
			{
				// ft_printf("str[%d] = %c, str[%d] = %c\n", i, str[i], j, str[j]);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

char *get_cmd_arguments(char *prompt)
{
	char *result;
	size_t i = 0;
	size_t total_len = ft_strlen(prompt);
	size_t res_len;
	size_t j;

	while (prompt[i] != ' ')
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

int cd_case(t_mshell *shell)
{
	char *str;
	int result;
	char *temp;

	shell->cmd_count++;
	str = get_cmd_arguments(shell->prompt);
	if (!str)
		return (1);
	temp = ft_strtrim(str, "\n");
	if (!temp)
		return (free(str), 1);
	result = chdir(temp);
	if (result != 0)
	{
		ft_printf("minishell: %d: can't cd to %s\n", shell->cmd_count, temp);
		return (free(str), free(temp), 1);
	}

	return (free(str), free(temp), 0);
}

int echo_case(char *prompt)
{
	char *str;
	char *temp;

	str = get_cmd_arguments(prompt);
	if (!str)
		return (1);
	are_quotes_closed(str);

	size_t i = 0;
	while (!are_quotes_closed(str))
	{
		i++;
		ft_putstr_fd("> ", 1);
		char *line = get_next_line(1);
		if (!line)
			return (free(str), 1);
		temp = ft_strjoin(str, line);
		if (!temp)
			return (free(line), free(str), 2);
		free(str);
		str = ft_strdup(temp);
		free(line);
		free(temp);
	}
	char *result;
	result = ft_strtrim(str, "\"\n\'");
	if (!result)
		return (3);
	ft_printf("%s\n", result);
	free(str);
	free(result);
	return (0);
}
int env_case(t_mshell shell, char **cmd_arr, char **envp)
{
	size_t j;
	char *temp;

	j = -1;
	while (shell.paths[++j])
	{
		ft_printf("env_case : cmd_arr[0] = %s\n", cmd_arr[0]);
		temp = ft_strjoin(shell.paths[j], cmd_arr[0]);
		if (!temp)
			return (2);
		execve(temp, cmd_arr, envp);
		free(temp);
	}
	return (ft_printf("sh: %d: %s: not found\n", shell.cmd_count, cmd_arr[0]), 1);
}

char *get_file(char *str, size_t i)
{

	(void)str;
	(void)i;

	return (NULL);
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

int redirect(t_mshell *shell)
{
	char *cmd;
	char *file;
	size_t i;

	ft_printf("got into redirect\n");

	i = 0;
	cmd = get_cmd(shell->prompt, &i);
	if (!cmd)
		return (1);

	ft_printf("redirect : cmd = %s\n");

	file = get_file(shell->prompt, i);
	(void)file;

	free(cmd);
	return (0);
}

/*to do :
- ENVP dans structure
- lister les builtins */
void execution(t_mshell *shell, char **envp)
{
	t_tokens *temp;
	pid_t	child;

	temp = shell->tok_lst;
	while (temp)
	{
		if (temp->type == CMD)
		{
			shell->cmd_count++;
			if (!ft_strncmp((const char *)temp->content, "echo", 4))
				echo_case(shell->prompt);
			else if (!ft_strncmp((const char *)temp->content, "cd", 2))
				cd_case(shell);
			else if (!ft_strncmp((const char *)temp->content, "exit", 4))
				return (free_struct(shell), exit(3));
			else
			{
				temp->cmd_arr = ft_split(temp->content, ' ');

				//ft_printf("cmd_arr[0] = %s, content = %s\n\n", temp->cmd_arr, temp->content);

				if (!temp->cmd_arr)
					return (free_struct(shell), exit(1));
				child = fork();
				if (!child)
				{
					if (env_case(*shell, temp->cmd_arr, envp))
						return (free_struct(shell), exit (4));
				}
				waitpid(child, NULL, 0);
				if (temp->cmd_arr)
					free_arr(temp->cmd_arr);
				temp->cmd_arr = NULL;
			}
		}
		temp = temp->next;
	}
}
