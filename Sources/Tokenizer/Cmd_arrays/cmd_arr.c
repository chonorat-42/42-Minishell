/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:24:51 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/06 19:24:51 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_status;

void	get_commands_lst(t_dlist *base, t_dlist **new)
{
	t_dlist	*temp;

	temp = base;
	while (temp)
	{
		if (is_char_in_set(temp->content[0], "<>"))
			temp = temp->next->next;
		else
		{
			split_into_dlst(new, temp->content, ft_strlen(temp->content), 0);
			temp = temp->next;
		}
	}
}

int	handle_fd(int fd, char *file, int type, t_tokens *temp)
{
	struct stat	sb;

	if (fd == -1 && type == CMD)
	{
		if (temp->fd_in_str)
			free(temp->fd_in_str);
		temp->fd_in_str = NULL;
		stat(file, &sb);
		if (errno == ENOENT)
			add_error(file, NO_FILE, temp);
		else if (errno == EACCES)
		{
			add_error(file, PERMISSIONS, temp);
			g_status = 1;
		}
		return (1);
	}
	return (0);
}

void	create_cmd_arr(t_tokens **tk_lst, t_mshell *shell)
{
	t_tokens	*temp;
	t_dlist		*new;

	temp = *tk_lst;
	while (temp)
	{
		new = NULL;
		get_commands_lst(temp->dlst, &new);
		if (!new)
			return (free_arr(shell->paths), shell->paths = NULL,
				free(shell->input), close_fd(shell), free_tokens(tk_lst),
				get_input_loop(shell));
		temp->cmd_arr = list_into_arr(new);
		free_dlist(&new);
		temp = temp->next;
	}
}
