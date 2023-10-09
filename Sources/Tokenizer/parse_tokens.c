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

	to_add = readline(">");
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

// int has_successive_pipes(t_tokens **tok)
// {
// 	t_tokens *temp;

// 	temp = *tok;
// 	while (temp)
// 	{
// 		if (temp->type == PIPE)
// 		{
// 			if (temp->next && temp->next->type == PIPE)
// 				return (1);
// 		}
// 		temp = temp->next;
// 	}
// 	return (0);
// }

void	parse_tkn(t_tokens **tok, t_mshell *shell)
{
	t_tokens	*temp;

	temp = *tok;
	if (temp->type == PIPE)
		return (ft_printf("minishell: syntax error near unexpected token '|'\n"), ft_free_tokens(&shell->tok_lst), get_input_loop(shell));
	while(last_is_pipe(*tok))
	{
		while (temp->next)
			temp = temp->next;
		if (temp->type == PIPE)
			hdoc_add_cmd(shell);
	}
	// if (has_successive_pipes(tok))
	// 	return (ft_printf("minishell: operator '||' not supported\n"), ft_free_tokens(&shell->tok_lst), get_input_loop(shell));
}