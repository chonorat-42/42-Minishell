/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:13:46 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/23 15:16:22 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_errors_single(t_tokens *token)
{
	t_error	*temp_err;

	if (token->errors)
	{
		temp_err = token->errors;
		while (temp_err)
		{
			if (temp_err->type == NO_FILE)
				show_error(temp_err->content, "NO_FILE", 0);
			else if (temp_err->type == PERMISSIONS)
				show_error(temp_err->content, "PERMISSION", 0);
			temp_err = temp_err->next;
		}
	}
}

void	print_errors(t_tokens *lst)
{
	t_tokens	*temp_tok;
	t_error		*temp_err;

	temp_tok = lst;
	while (temp_tok)
	{
		if (temp_tok->errors)
		{
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
