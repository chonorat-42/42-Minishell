/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:29 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/23 14:28:00 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

void	print_errors_single(t_tokens *token)
{
	t_error	*temp_err;
	if (token->errors)
	{
		// ft_dprintf(2, "temp_tok->errors exists\n");
		temp_err = token->errors;
		// ft_putstr_fd("\033[0;37m\033[1mminishell: ", 2);
		while (temp_err)
		{
			if (temp_err->type == NO_FILE)
					show_error(temp_err->content, "NO_FILE", 0);
				else if (temp_err->type == PERMISSIONS)
					show_error(temp_err->content, "PERMISSION", 0);
			temp_err = temp_err->next;
		}
		ft_dprintf(2, "\033[0m");
	}
}

void	print_errors(t_tokens *lst)
{
	t_tokens	*temp_tok;
	t_error		*temp_err;

	// ft_dprintf(2, "got in print errors\n");
	temp_tok = lst;
	while (temp_tok)
	{
		if (temp_tok->errors)
		{
			// ft_dprintf(2, "temp_tok->errors exists\n");
			temp_err = temp_tok->errors;
			while (temp_err)
			{
				if (temp_err->type == NO_FILE)
					show_error(temp_err->content, "NO_FILE", 0);
				else if (temp_err->type == PERMISSIONS)
					show_error(temp_err->content, "PERMISSION", 0);
				temp_err = temp_err->next;
			}
		}
		temp_tok = temp_tok->next;
	}
	ft_dprintf(2, "\033[0m");
}
void	error_addback(t_tokens *tok, char *file, int type)
{
	t_error	*new;
	t_error	*temp;

	new = malloc(sizeof(*new));
	if (!tok->errors)
		tok->errors = new;
	else
	{
		temp = tok->errors;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
	new->type = type;
	new->content = ft_strdup(file);
	new->next = NULL;
}

void	add_error(char *file, int type, t_tokens *temp)
{
	if (type == NO_FILE || type == PERMISSIONS)
		error_addback(temp, file, type);

}

static void	exec_error(char *cmd, int error)
{
	if (error == 1)
		ft_dprintf(2, "%s: command not found\n", cmd);
	else if (error == 2)
		ft_dprintf(2, "%s: No such file or directory\n", cmd);
	if (error == 3)
	{
		ft_dprintf(2, "%s: Is a directory\n", cmd);
		g_status = 126;
		return ;
	}
	g_status = 127;
}

static void	heredoc_error(char *delimiter)
{
	ft_dprintf(2, "warning: ");
	ft_dprintf(2, "here-document delimited by end-of-file ");
	ft_dprintf(2, "(wanted `%s')\n", delimiter);
}

void	show_error(char *cmd, char *type, int error)
{
	ft_putstr_fd("\033[0;37m\033[1mminishell: ", 2);
	if (ft_strcmp(type, "EXEC") == 0)
		exec_error(cmd, error);
	else if (ft_strcmp(type, "QUOTES") == 0)
	{
		ft_putendl_fd("Quotes are not closed", 2);
		g_status = 1;
	}
	else if (ft_strcmp(type, "HEREDOC") == 0)
		heredoc_error(cmd);
	else if (ft_strcmp(type, "NO_FILE") == 0)
	{
		ft_dprintf(2, "%s: No such file or directory\n", cmd);
		g_status = 1;
	}
	else if (ft_strcmp(type, "PERMISSION") == 0)
		ft_dprintf(2, "%s: Permission denied\n", cmd);
	else if (ft_strcmp(type, "SYNTAX") == 0)
	{
		ft_dprintf(2, "syntax error near unexpected token `%s'\n", cmd);
		g_status = 2;
	}
	else if (ft_strcmp(type, "SHLVL") == 0)
		ft_dprintf(2, "warning: shell level (%d) too high, resetting to 1\n",
			error);
	ft_printf("\033[0m");
}
