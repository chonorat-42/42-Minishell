/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:05:39 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/10/06 19:05:39 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hdoc_add_cmd(t_mshell *shell)
{
	char	*to_add;
	char	*trim;

	ft_dprintf(STDOUT_FILENO, ">");
	to_add = get_next_line(0);
	if (!to_add)
		return ((void)ft_putstr_fd("\n", 1), free(shell->input),
			free_arr(shell->paths), shell->paths = NULL, get_input_loop(shell));
	trim = ft_strtrim(to_add, "\n");
	free(to_add);
	to_add = trim;
	split_on_pipes(shell, to_add);
	free(to_add);
}

int	last_is_pipe(t_tokens *tok)
{
	t_tokens	*temp;

	temp = tok;
	while (temp->next)
		temp = temp->next;
	if (temp->type == PIPE)
		return (1);
	return (0);
}

void	parse_tkn(t_tokens **tok, t_mshell *shell)
{
	t_tokens	*temp;

	temp = *tok;

	if (temp->type == PIPE)
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token `|'\n");
		return (ft_free_tokens(&shell->tok_lst), get_input_loop(shell));
	}
}
