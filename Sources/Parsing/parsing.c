/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 18:13:13 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 18:24:53 by pgouasmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	are_all_quotes_closed(char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"'))
		{
			j = i + 1;
			while (str[j] && str[j] != str[i])
				j++;
			if (!str[j])
				return (0);
			i = j;
		}
		i++;
	}
	return (1);
}

void	add_quotes_input(t_mshell *shell)
{
	char	*line;
	char	*join;

	ft_dprintf(STDOUT_FILENO, ">");
	line = get_next_line(0);
	if (!line)
		return ((void)ft_putstr_fd("\n", 1), free(shell->input),
			free_arr(shell->paths), shell->paths = NULL, get_input_loop(shell));
	join = ft_strjoin(shell->input, line);
	if (!join)
		return (free(line), free_struct(shell), exit(2));
	free(line);
	free(shell->input);
	shell->input = join;
}

void	parsing(t_mshell *shell)
{
	char	*new_input;

	if (!are_all_quotes_closed(shell->input))
	{
		new_input = ft_strjoin(shell->input, "\n");
		free(shell->input);
		shell->input = new_input;
	}
	while (!are_all_quotes_closed(shell->input))
		add_quotes_input(shell);
	if (shell->input[0] != '\0')
	{
		if (expand(shell, shell->input) == 1)
			return (free(shell->input), get_input_loop(shell));
	}
	else
	{
		free(shell->input);
		shell->input = NULL;
		get_input_loop(shell);
	}
}
